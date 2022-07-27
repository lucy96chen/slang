// debug-fence.cpp
#include "debug-fence.h"

namespace gfx
{
using namespace Slang;

namespace debug
{

Result DebugFence::getSharedHandle(InteropHandle* outHandle)
{
    SLANG_GFX_API_FUNC;
    return baseObject->getSharedHandle(outHandle);
}

Result DebugFence::getNativeHandle(InteropHandle* outNativeHandle)
{
    SLANG_GFX_API_FUNC;
    return baseObject->getNativeHandle(outNativeHandle);
}

Result DebugFence::getCurrentValue(uint64_t* outValue)
{
    SLANG_GFX_API_FUNC;
    return baseObject->getCurrentValue(outValue);
}

Result DebugFence::setCurrentValue(uint64_t value)
{
    SLANG_GFX_API_FUNC;
    return baseObject->setCurrentValue(value);
}

} // namespace debug
} // namespace gfx
