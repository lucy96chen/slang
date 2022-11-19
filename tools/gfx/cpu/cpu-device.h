// cpu-device.h
#pragma once
#include "cpu-base.h"

#include "cpu-pipeline-state.h"
#include "cpu-shader-object.h"

namespace gfx
{
using namespace Slang;

namespace cpu
{

class DeviceImpl : public ImmediateComputeDeviceBase
{
public:
    ~DeviceImpl();

    virtual SLANG_NO_THROW SlangResult SLANG_MCALL initialize(const Desc& desc) override;

    virtual SLANG_NO_THROW Result SLANG_MCALL createTextureResource(
        const ITextureResource::Desc& desc,
        const ITextureResource::SubresourceData* initData,
        ITextureResource** outResource) override;

    virtual SLANG_NO_THROW Result SLANG_MCALL createBufferResource(
        const IBufferResource::Desc& descIn,
        const void* initData,
        IBufferResource** outResource) override;

    virtual SLANG_NO_THROW Result SLANG_MCALL createTextureView(
        ITextureResource* inTexture, IResourceView::Desc const& desc, IResourceView** outView) override;

    virtual SLANG_NO_THROW Result SLANG_MCALL createBufferView(
        IBufferResource* inBuffer,
        IBufferResource* counterBuffer,
        IResourceView::Desc const& desc,
        IResourceView** outView) override;

    virtual SLANG_NO_THROW Result SLANG_MCALL createFramebuffer(
        const IFramebuffer::Desc& desc,
        IFramebuffer** outFramebuffer) override;

    virtual SLANG_NO_THROW Result SLANG_MCALL createFramebufferLayout(
        const IFramebufferLayout::Desc& desc,
        IFramebufferLayout** outLayout) override;

    virtual Result createShaderObjectLayout(
        slang::TypeLayoutReflection* typeLayout,
        ShaderObjectLayoutBase** outLayout) override;

    virtual Result createShaderObject(
        ShaderObjectLayoutBase* layout,
        IShaderObject** outObject) override;

    virtual Result createMutableShaderObject(
        ShaderObjectLayoutBase* layout,
        IShaderObject** outObject) override;

    virtual Result createRootShaderObject(IShaderProgram* program, ShaderObjectBase** outObject) override;

    virtual SLANG_NO_THROW Result SLANG_MCALL createMutableRootShaderObject(
        IShaderProgram* program,
        IShaderObject** outObject) override;

    virtual SLANG_NO_THROW Result SLANG_MCALL createProgram(
        const IShaderProgram::Desc& desc,
        IShaderProgram** outProgram,
        ISlangBlob** outDiagnosticBlob) override;

    virtual SLANG_NO_THROW Result SLANG_MCALL createComputePipelineState(
        const ComputePipelineStateDesc& desc, IPipelineState** outState) override;

    virtual SLANG_NO_THROW Result SLANG_MCALL createQueryPool(
        const IQueryPool::Desc& desc, IQueryPool** outPool) override;

    virtual void writeTimestamp(IQueryPool* pool, GfxIndex index) override;

    virtual SLANG_NO_THROW const DeviceInfo& SLANG_MCALL getDeviceInfo() const override;

    virtual SLANG_NO_THROW Result SLANG_MCALL
        createSamplerState(ISamplerState::Desc const& desc, ISamplerState** outSampler) override;

    virtual SLANG_NO_THROW Result SLANG_MCALL createSwapchain(
        const ISwapchain::Desc& desc,
        WindowHandle window,
        ISwapchain** outSwapchain) override;

    virtual SLANG_NO_THROW Result SLANG_MCALL
        createFence(const IFence::Desc& desc, IFence** outFence) override;

    virtual SLANG_NO_THROW Result SLANG_MCALL waitForFences(
        GfxCount fenceCount,
        IFence** fences,
        uint64_t* values,
        bool waitForAll,
        uint64_t timeout) override;

    virtual SLANG_NO_THROW Result SLANG_MCALL createTransientResourceHeap(
        const ITransientResourceHeap::Desc& desc,
        ITransientResourceHeap** outHeap) override;

    virtual SLANG_NO_THROW Result SLANG_MCALL
        createCommandQueue(const ICommandQueue::Desc& desc, ICommandQueue** outQueue) override;

    virtual void submitGpuWork() override {}
    virtual void waitForGpu() override {}
    virtual void* map(IBufferResource* buffer, MapFlavor flavor) override;
    virtual void unmap(IBufferResource* buffer, size_t offsetWritten, size_t sizeWritten) override;

    virtual void signalFence(IFence* fence, uint64_t valueToSignal) override;

    virtual void setPipelineState(IPipelineState* state) override;

    virtual void bindRootShaderObject(IShaderObject* object) override;

    virtual void dispatchCompute(int x, int y, int z) override;

    virtual void copyBuffer(
        IBufferResource* dst,
        size_t dstOffset,
        IBufferResource* src,
        size_t srcOffset,
        size_t size) override;

private:
    RefPtr<PipelineStateImpl> m_currentPipeline = nullptr;
    RefPtr<RootShaderObjectImpl> m_currentRootObject = nullptr;
    DeviceInfo m_info;
};

} // namespace cpu

Result SLANG_MCALL createCPUDevice(const IDevice::Desc* desc, IDevice** outDevice);

} // namespace gfx
