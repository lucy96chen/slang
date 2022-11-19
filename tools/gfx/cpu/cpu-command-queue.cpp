#include "cpu-command-queue.h"

#include "cpu-command-buffer.h"

namespace gfx
{

using namespace Slang;

namespace cpu
{
    ICommandQueue* CommandQueueImpl::getInterface(const Guid& guid)
    {
        if (guid == GfxGUID::IID_ISlangUnknown || guid == GfxGUID::IID_ICommandQueue)
            return static_cast<ICommandQueue*>(this);
        return nullptr;
    }

    void CommandQueueImpl::init(DeviceImpl* renderer, uint32_t queueIndex)
    {
        m_renderer = renderer;
        m_queueIndex = queueIndex;
    }

    void CommandQueueImpl::waitOnHost()
    {

    }

    Result CommandQueueImpl::getNativeHandle(InteropHandle* outHandle)
    {
        return SLANG_OK;
    }

    Result CommandQueueImpl::waitForFenceValuesOnDevice(
        GfxCount fenceCount, IFence** fences, uint64_t* waitValues)
    {
        return SLANG_OK;
    }

    void CommandQueueImpl::executeCommandBuffers(
        GfxCount count,
        ICommandBuffer* const* commandBuffers,
        IFence* fence,
        uint64_t valueToSignal)
    {
        for (GfxIndex i = 0; i < count; ++i)
        {
            execute(static_cast<CommandBufferImpl*>(commandBuffers[i]));
        }

        if (fence)
        {
            m_renderer->signalFence(fence, valueToSignal);
        }
    }

    void CommandQueueImpl::execute(CommandBufferImpl* commandBuffer)
    {
        for (auto& cmd : commandBuffer->m_commands)
        {
            switch (cmd.name)
            {
            case CommandName::SetPipelineState:
                m_renderer->setPipelineState(commandBuffer->getObject<PipelineStateBase>(cmd.operands[0]));
                break;
            case CommandName::BindRootShaderObject:
                m_renderer->bindRootShaderObject(
                    commandBuffer->getObject<ShaderObjectBase>(cmd.operands[0]));
                break;
            case CommandName::DispatchCompute:
                m_renderer->dispatchCompute(
                    int(cmd.operands[0]), int(cmd.operands[1]), int(cmd.operands[2]));
            }
        }
    }
}

}
