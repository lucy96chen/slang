// slang-shader-cache-index.cpp
#include "persistent-shader-cache.h"

#include "../../source/core/slang-digest-util.h"
#include "../../source/core/slang-io.h"
#include "../../source/core/slang-string-util.h"
#include "../../source/core/slang-file-system.h"

namespace Slang
{

PersistentShaderCache::PersistentShaderCache(const Desc& desc)
{
    entryCountLimit = desc.entryCountLimit;
    shaderCacheFileSystem = desc.shaderCacheFileSystem;
    cacheFilename = desc.indexFilename;

    if (!desc.shaderCachePath.getBuffer())
    {
        if (!shaderCacheFileSystem)
        {
            // Only a path was provided, so we get a mutable file system
            // using OSFileSystem::getMutableSingleton.
            shaderCacheFileSystem = OSFileSystem::getMutableSingleton();
        }
        shaderCacheFileSystem = new RelativeFileSystem(shaderCacheFileSystem, desc.shaderCachePath);
    }

    // If our shader cache has an underlying file system, check if it's mutable. If so, store a pointer
    // to the mutable version in order to save new entries later.
    if (shaderCacheFileSystem)
    {
        shaderCacheFileSystem->queryInterface(ISlangMutableFileSystem::getTypeGuid(), (void**)mutableShaderCacheFileSystem.writeRef());
    }

    loadCacheFromFile();
}

// Load a previous cache index saved to disk. If not found, create a new cache index
// and save it to disk as filename.
void PersistentShaderCache::loadCacheFromFile()
{
    ComPtr<ISlangBlob> indexBlob;
    if (SLANG_FAILED(shaderCacheFileSystem->loadFile(cacheFilename.getBuffer(), indexBlob.writeRef())))
    {
        // Cache index not found, so we'll create and save a new one.
        if (mutableShaderCacheFileSystem)
        {
            mutableShaderCacheFileSystem->saveFile(cacheFilename.getBuffer(), nullptr, 0);
            return;
        }
        // Cache index not found and we can't save a new one due to the file system being immutable.
        return;
    }

    String indexString;
    File::readAllText(cacheFilename, indexString);

    List<UnownedStringSlice> lines;
    StringUtil::calcLines(indexString.getUnownedSlice(), lines);
    for (auto line : lines)
    {
        List<UnownedStringSlice> digests;
        StringUtil::split(line, ' ', digests); // This will return our two hashes as two elements in digests.
        auto dependencyDigest = DigestUtil::fromString(digests[0]);
        auto astDigest = DigestUtil::fromString(digests[1]);

        ShaderCacheEntry entry = { dependencyDigest, astDigest };
        auto entryNode = entries.AddLast(entry);
        keyToEntry.Add(dependencyDigest, entryNode);
    }
}

LinkedNode<ShaderCacheEntry>* PersistentShaderCache::findEntry(const slang::Digest& key, ISlangBlob** outCompiledCode)
{
    LinkedNode<ShaderCacheEntry>* entryNode;
    if (!keyToEntry.TryGetValue(key, entryNode))
    {
        // The key was not found in the cache, so we return nullptr.
        *outCompiledCode = nullptr;
        return nullptr;
    }

    // If the key is found, load the stored contents from disk. We then move the corresponding
    // entry to the front of the linked list and update the cache file on disk
    shaderCacheFileSystem->loadFile(DigestUtil::toString(key).getBuffer(), outCompiledCode);
    if (entries.FirstNode() != entryNode)
    {
        entries.RemoveFromList(entryNode);
        entries.AddFirst(entryNode);
        if (mutableShaderCacheFileSystem)
        {
            saveCacheToFile();
        }
    }
    return entryNode;
}

void PersistentShaderCache::addEntry(const slang::Digest& dependencyDigest, const slang::Digest& astDigest, ISlangBlob* compiledCode)
{
    if (!mutableShaderCacheFileSystem)
    {
        // Should not save new entries if the underlying file system isn't mutable.
        return;
    }
    
    // Check that we do not exceed the cache's size limit by adding another entry. If so,
    // remove the least recently used entry first.
    //
    // In theory, this could loop infinitely since deleteLRUEntry() immediately returns if
    // mutableShaderCacheFileSystem is not set. However, this situation is functionally impossible
    // because we check immediately before this as well.
    while (entryCountLimit > 0 && entries.Count() >= entryCountLimit)
    {
        deleteLRUEntry();
    }

    ShaderCacheEntry entry = { dependencyDigest, astDigest };
    auto entryNode = entries.AddFirst(entry);
    keyToEntry.Add(dependencyDigest, entryNode);

    mutableShaderCacheFileSystem->saveFileBlob(DigestUtil::toString(dependencyDigest).getBuffer(), compiledCode);

    saveCacheToFile();
}

void PersistentShaderCache::updateEntry(
    LinkedNode<ShaderCacheEntry>* entryNode,
    const slang::Digest& dependencyDigest,
    const slang::Digest& astDigest,
    ISlangBlob* updatedCode)
{
    if (!mutableShaderCacheFileSystem)
    {
        // Updating entries requires saving to disk in order to overwrite the old shader file
        // on disk, so we return if the underlying file system isn't mutable.
        return;
    }

    entryNode->Value.astBasedDigest = astDigest;
    mutableShaderCacheFileSystem->saveFileBlob(DigestUtil::toString(dependencyDigest).getBuffer(), updatedCode);

    saveCacheToFile();
}

void PersistentShaderCache::saveCacheToFile()
{
    if (!mutableShaderCacheFileSystem)
    {
        // Cannot save the index to disk if the underlying file system isn't mutable.
        return;
    }

    StringBuilder indexSb;
    for (auto& entry : entries)
    {
        indexSb << entry.dependencyBasedDigest;
        indexSb << " ";
        indexSb << entry.astBasedDigest;
        indexSb << "\n";
    }

    mutableShaderCacheFileSystem->saveFile(cacheFilename.getBuffer(), indexSb.getBuffer(), indexSb.getLength());
}

void PersistentShaderCache::deleteLRUEntry()
{
    if (!mutableShaderCacheFileSystem)
    {
        // This is here as a safety precaution but should never be hit as
        // addEntry() is the only function that should call this.
        return;
    }

    auto lruEntry = entries.LastNode();
    auto shaderKey = lruEntry->Value.dependencyBasedDigest;

    keyToEntry.Remove(shaderKey);
    mutableShaderCacheFileSystem->remove(DigestUtil::toString(shaderKey).getBuffer());

    entries.Delete(lruEntry);
}

}
