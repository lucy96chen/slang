#include "cpu-transient-heap.h"

#include "cpu-command-buffer.h"

namespace gfx
{

using namespace Slang;

namespace cpu
{
    Result TransientResourceHeapImpl::init(const ITransientResourceHeap::Desc& desc, DeviceImpl* device)
    {
        m_device = device;
        return SLANG_OK;
    }

    Result TransientResourceHeapImpl::createCommandBuffer(ICommandBuffer** outCommandBuffer)
    {
        RefPtr<CommandBufferImpl> commandBuffer = new CommandBufferImpl();
        commandBuffer->init(m_device, this);
        returnComPtr(outCommandBuffer, commandBuffer);
        return SLANG_OK;
    }

    Result TransientResourceHeapImpl::synchronizeAndReset()
    {
        return SLANG_OK;
    }
}

}
