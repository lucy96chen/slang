#pragma once

#include "cpu-base.h"

#include "cpu-device.h"

namespace gfx
{

using namespace Slang;

namespace cpu
{

class CommandQueueImpl : public ICommandQueue, public ComObject
{
public:
    SLANG_COM_OBJECT_IUNKNOWN_ALL
        ICommandQueue* getInterface(const Guid& guid);

    void init(DeviceImpl* renderer, uint32_t queueIndex);

    virtual SLANG_NO_THROW void SLANG_MCALL waitOnHost() override;

    virtual SLANG_NO_THROW Result SLANG_MCALL getNativeHandle(InteropHandle* outHandle) override;

    virtual SLANG_NO_THROW const Desc& SLANG_MCALL getDesc() override { return m_desc; }

    virtual SLANG_NO_THROW Result SLANG_MCALL waitForFenceValuesOnDevice(
        GfxCount fenceCount, IFence** fences, uint64_t* waitValues) override;

    virtual SLANG_NO_THROW void SLANG_MCALL executeCommandBuffers(
        GfxCount count,
        ICommandBuffer* const* commandBuffers,
        IFence* fence,
        uint64_t valueToSignal) override;

    void execute(CommandBufferImpl* commandBuffer);

private:
    Desc m_desc;
    RefPtr<DeviceImpl> m_renderer;
    uint32_t m_queueIndex;
};

}
}
