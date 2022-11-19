#include "cpu-command-buffer.h"

namespace gfx
{

using namespace Slang;

namespace cpu
{

ICommandBuffer* CommandBufferImpl::getInterface(const Guid& guid)
{
    if (guid == GfxGUID::IID_ISlangUnknown || guid == GfxGUID::IID_ICommandBuffer)
        return static_cast<ICommandBuffer*>(this);
    return nullptr;
}

void CommandBufferImpl::init(DeviceImpl* device, TransientResourceHeapImpl* transientHeap)
{
    m_device = device;
    m_transientHeap = transientHeap;
}

void CommandBufferImpl::encodeRenderCommands(
    IRenderPassLayout* renderPass,
    IFramebuffer* framebuffer,
    IRenderCommandEncoder** outEncoder)
{
    SLANG_UNUSED(renderPass);
    SLANG_UNUSED(framebuffer);
    *outEncoder = nullptr;
}

void CommandBufferImpl::encodeResourceCommands(IResourceCommandEncoder** outEncoder)
{
    m_resourceCommandEncoder.init(this);
    *outEncoder = &m_resourceCommandEncoder;
}

void CommandBufferImpl::encodeComputeCommands(IComputeCommandEncoder** outEncoder)
{
    m_computeCommandEncoder.init(m_device, m_transientHeap, this);
    *outEncoder = &m_computeCommandEncoder;
}

void CommandBufferImpl::encodeRayTracingCommands(IRayTracingCommandEncoder** outEncoder)
{
    *outEncoder = nullptr;
}

Result CommandBufferImpl::getNativeHandle(InteropHandle* outHandle)
{
    return SLANG_FAIL;
}

}
}
