#pragma once

#include "slang-gfx.h"
#include "../renderer-shared.h"
#include "cpu-base.h"

namespace gfx
{

namespace cpu
{
    class FenceImpl : public FenceBase
    {
    public:
        Result init(DeviceImpl* device, const IFence::Desc& desc)
        {
            SLANG_UNUSED(device);
            m_desc = desc;
            m_value = m_desc.initialValue;
            return SLANG_OK;
        }

        virtual SLANG_NO_THROW Result SLANG_MCALL getCurrentValue(uint64_t* outValue) override
        {
            *outValue = m_value;
            return SLANG_OK;
        }

        virtual SLANG_NO_THROW Result SLANG_MCALL setCurrentValue(uint64_t value) override
        {
            m_value = value;
            return SLANG_OK;
        }

        virtual SLANG_NO_THROW Result SLANG_MCALL getSharedHandle(InteropHandle* outHandle) override
        {
            outHandle = nullptr;
            return SLANG_OK;
        }

        virtual SLANG_NO_THROW Result SLANG_MCALL
            getNativeHandle(InteropHandle* outNativeHandle) override
        {
            outNativeHandle = nullptr;
            return SLANG_OK;
        }

    private:
        IFence::Desc m_desc;
        uint64_t m_value = 0;
    };
}

}
