#pragma once

#include "cpu-base.h"
#include "cpu-command-encoder.h"

namespace gfx
{

using namespace Slang;

namespace cpu
{

class CommandBufferImpl : public ICommandBuffer, public CommandWriter, public ComObject
{
public:
    SLANG_COM_OBJECT_IUNKNOWN_ALL
        ICommandBuffer* getInterface(const Guid& guid);

    DeviceImpl* m_device;
    TransientResourceHeapImpl* m_transientHeap;
    ResourceCommandEncoderImpl m_resourceCommandEncoder;
    ComputeCommandEncoderImpl m_computeCommandEncoder;

    void init(DeviceImpl* device, TransientResourceHeapImpl* transientHeap);
    virtual SLANG_NO_THROW void SLANG_MCALL encodeRenderCommands(
        IRenderPassLayout* renderPass,
        IFramebuffer* framebuffer,
        IRenderCommandEncoder** outEncoder) override;

    virtual SLANG_NO_THROW void SLANG_MCALL
        encodeResourceCommands(IResourceCommandEncoder** outEncoder) override;
    virtual SLANG_NO_THROW void SLANG_MCALL
        encodeComputeCommands(IComputeCommandEncoder** outEncoder) override;
    virtual SLANG_NO_THROW void SLANG_MCALL
        encodeRayTracingCommands(IRayTracingCommandEncoder** outEncoder) override;

    virtual SLANG_NO_THROW void SLANG_MCALL close() override {}

    virtual SLANG_NO_THROW Result SLANG_MCALL getNativeHandle(InteropHandle* outHandle) override;
};

}
}
