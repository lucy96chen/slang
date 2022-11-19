#pragma once

#include "cpu-base.h"
#include "cpu-buffer.h"
#include "cpu-device.h"

namespace gfx
{

using namespace Slang;

namespace cpu
{

class TransientResourceHeapImpl : public TransientResourceHeapBaseImpl<DeviceImpl, BufferResourceImpl>
{
public:
    Result init(const ITransientResourceHeap::Desc& desc, DeviceImpl* device);

    virtual SLANG_NO_THROW Result SLANG_MCALL
        createCommandBuffer(ICommandBuffer** outCommandBuffer) override;
    virtual SLANG_NO_THROW Result SLANG_MCALL synchronizeAndReset() override;

    RefPtr<DeviceImpl> m_device;
};

}

}
