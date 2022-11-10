// cpu-swapchain.h
#pragma once

#include "slang-gfx.h"
#include "../renderer-shared.h"

namespace gfx
{

namespace cpu
{
    class SwapchainImpl
        : public ISwapchain
        , public Slang::ComObject
    {
    public:
        SLANG_COM_OBJECT_IUNKNOWN_ALL
        ISwapchain* getInterface(const Slang::Guid& guid)
        {
            if (guid == GfxGUID::IID_ISlangUnknown || guid == GfxGUID::IID_ISwapchain)
                return static_cast<ISwapchain*>(this);
            return nullptr;
        }

        virtual SLANG_NO_THROW const Desc& SLANG_MCALL getDesc() override { return m_desc; }

        /// Returns the back buffer image at `index`.
        virtual SLANG_NO_THROW Result SLANG_MCALL
            getImage(GfxIndex index, ITextureResource** outResource) override
        {
            SLANG_UNUSED(index);
            *outResource = nullptr;
            return SLANG_OK;
        }

        /// Present the next image in the swapchain.
        virtual SLANG_NO_THROW Result SLANG_MCALL present() override { return SLANG_OK; }

        /// Returns the index of next back buffer image that will be presented in the next
        /// `present` call. If the swapchain is invalid/out-of-date, this method returns -1.
        virtual SLANG_NO_THROW int SLANG_MCALL acquireNextImage() override { return 0; }

        /// Resizes the back buffers of this swapchain. All render target views and framebuffers
        /// referencing the back buffer images must be freed before calling this method.
        virtual SLANG_NO_THROW Result SLANG_MCALL resize(GfxCount width, GfxCount height) override
        {
            SLANG_UNUSED(width);
            SLANG_UNUSED(height);
            return SLANG_OK;
        }

        // Check if the window is occluded.
        virtual SLANG_NO_THROW bool SLANG_MCALL isOccluded() override { return false; }

        // Toggle full screen mode.
        virtual SLANG_NO_THROW Result SLANG_MCALL setFullScreenMode(bool mode) override
        {
            SLANG_UNUSED(mode);
            return SLANG_OK;
        }

        Result init(const ISwapchain::Desc& desc, WindowHandle window)
        {
            SLANG_UNUSED(window);
            m_desc = desc;
            return SLANG_OK;
        }

    private:
        ISwapchain::Desc m_desc;
    };
    
}

}
