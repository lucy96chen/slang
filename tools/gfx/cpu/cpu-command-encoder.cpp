#include "cpu-command-encoder.h"

#include "cpu-command-buffer.h"
#include "cpu-device.h"
#include "cpu-texture.h"
#include "cpu-transient-heap.h"

namespace gfx
{
namespace cpu
{

void ResourceCommandEncoderImpl::init(CommandBufferImpl* cmdBuffer)
{
    m_commandBuffer = cmdBuffer;
}

void ResourceCommandEncoderImpl::copyBuffer(
    IBufferResource* dst,
    Offset dstOffset,
    IBufferResource* src,
    Offset srcOffset,
    Size size)
{

}

void ResourceCommandEncoderImpl::uploadBufferData(IBufferResource* dst, Offset offset, Size size, void* data)
{

}

void ResourceCommandEncoderImpl::textureBarrier(
    GfxCount count,
    ITextureResource* const* textures,
    ResourceState src,
    ResourceState dst)
{

}

void ResourceCommandEncoderImpl::bufferBarrier(
    GfxCount count,
    IBufferResource* const* buffers,
    ResourceState src,
    ResourceState dst)
{

}

void ResourceCommandEncoderImpl::writeTimestamp(IQueryPool* pool, GfxIndex index)
{

}

void ResourceCommandEncoderImpl::copyTexture(
    ITextureResource* dst,
    ResourceState dstState,
    SubresourceRange dstSubresource,
    ITextureResource::Offset3D dstOffset,
    ITextureResource* src,
    ResourceState srcState,
    SubresourceRange srcSubresource,
    ITextureResource::Offset3D srcOffset,
    ITextureResource::Extents extent)
{

}

// TODO: Should this do anything if invalid parameters are passed in (extents/offsets would
// exceed the boundaries of the destination texture or is it the user's responsibility to
// pass in valid parameters?
void ResourceCommandEncoderImpl::uploadTextureData(
    ITextureResource* dst,
    SubresourceRange subResourceRange,
    ITextureResource::Offset3D offset,
    ITextureResource::Extents extent,
    ITextureResource::SubresourceData* subResourceData,
    GfxCount subResourceDataCount)
{
    auto dstTexture = static_cast<TextureResourceImpl*>(dst);
    auto baseSubresourceIndex = subResourceRange.mipLevel
        + subResourceRange.baseArrayLayer * dstTexture->getDesc()->numMipLevels;
    auto textureSize = dstTexture->getDesc()->size;

    FormatInfo formatInfo = {};
    gfxGetFormatInfo(dstTexture->getDesc()->format, &formatInfo);

    for (GfxCount i = 0; i < subResourceDataCount; ++i)
    {
        auto subresourceIndex = baseSubresourceIndex + i;
        uint32_t mipLevel = subresourceIndex % dstTexture->getDesc()->numMipLevels;
        auto copyWidth = (extent.width == ITextureResource::kRemainingTextureSize)
            ? Math::Max(1, (textureSize.width >> mipLevel)) - offset.x : extent.width;
        auto copyHeight = (extent.height == ITextureResource::kRemainingTextureSize)
            ? Math::Max(1, (textureSize.height >> mipLevel)) - offset.y : extent.height;
        auto copyDepth = (extent.depth == ITextureResource::kRemainingTextureSize)
            ? Math::Max(1, (textureSize.depth >> mipLevel)) - offset.z : extent.depth;
        auto rowSize = (copyWidth + formatInfo.blockWidth - 1) /
            formatInfo.blockWidth * formatInfo.blockSizeInBytes;
        auto rowCount =
            (copyHeight + formatInfo.blockHeight - 1) / formatInfo.blockHeight;

        for (int32_t z = 0; z < copyDepth; ++z)
        {
            auto imageStart = (uint8_t*)dstTexture->m_data + rowSize * rowCount * (z + offset.z);
            auto srcData = (uint8_t*)subResourceData->data + subResourceData->strideZ * z;
            for (int32_t row = 0; row < rowCount; ++row)
            {
                memcpy(imageStart + (row + offset.y) * rowSize + offset.x,
                       srcData + subResourceData->strideY * row,
                       rowSize);
            }
        }
    }
}

void ResourceCommandEncoderImpl::clearResourceView(
    IResourceView* view, ClearValue* clearValue, ClearResourceViewFlags::Enum flags)
{

}

void ResourceCommandEncoderImpl::resolveResource(
    ITextureResource* source,
    ResourceState sourceState,
    SubresourceRange sourceRange,
    ITextureResource* dest,
    ResourceState destState,
    SubresourceRange destRange)
{

}

void ResourceCommandEncoderImpl::resolveQuery(
    IQueryPool* queryPool,
    GfxIndex index,
    GfxCount count,
    IBufferResource* buffer,
    Offset offset)
{

}

void ResourceCommandEncoderImpl::copyTextureToBuffer(
    IBufferResource* dst,
    Offset dstOffset,
    Size dstSize,
    Size dstRowStride,
    ITextureResource* src,
    ResourceState srcState,
    SubresourceRange srcSubresource,
    ITextureResource::Offset3D srcOffset,
    ITextureResource::Extents extent)
{

}

void ResourceCommandEncoderImpl::textureSubresourceBarrier(
    ITextureResource* texture,
    SubresourceRange subresourceRange,
    ResourceState src,
    ResourceState dst)
{

}

void ResourceCommandEncoderImpl::beginDebugEvent(const char* name, float rgbColor[3])
{

}

void ResourceCommandEncoderImpl::endDebugEvent()
{

}

void ComputeCommandEncoderImpl::endEncoding()
{

}

void ComputeCommandEncoderImpl::init(
    DeviceImpl* renderer,
    TransientResourceHeapImpl* transientHeap,
    CommandBufferImpl* cmdBuffer)
{
    m_commandBuffer = cmdBuffer;
    m_transientHeap = transientHeap;
    m_currentPipeline = nullptr;
    m_writer = cmdBuffer;
}

Result ComputeCommandEncoderImpl::bindPipeline(IPipelineState* state, IShaderObject** outRootObject)
{
    SLANG_UNUSED(state);
    SLANG_UNUSED(outRootObject);
    return SLANG_OK;
}

Result ComputeCommandEncoderImpl::bindPipelineWithRootObject(IPipelineState* state, IShaderObject* rootObject)
{
    m_writer->setPipelineState(state);
    PipelineStateBase* pipelineImpl = static_cast<PipelineStateBase*>(state);
    SLANG_RETURN_ON_FAIL(m_commandBuffer->m_device->createRootShaderObject(
        pipelineImpl->m_program, m_rootObject.writeRef()));
    m_rootObject->copyFrom(rootObject, m_commandBuffer->m_transientHeap);
    return SLANG_OK;
}

void ComputeCommandEncoderImpl::dispatchCompute(int x, int y, int z)
{
    m_writer->bindRootShaderObject(m_rootObject);
    m_writer->dispatchCompute(x, y, z);
}

void ComputeCommandEncoderImpl::dispatchComputeIndirect(IBufferResource* argBuffer, Offset offset)
{

}

}
}
