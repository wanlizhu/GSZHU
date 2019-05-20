#pragma once

#include <GSZHU/IObject.h>
#include <GSZHU/ITextureView.h>
#include <GSZHU/DESC/SSwapChainDesc.h>
#include <GSZHU/SDisplayModeAttribs.h>

namespace GSZHU {
    // The swap chain is created by a platform-dependent function
    class GSZHU_API ISwapChain : public IObject {
    public:
        using DESC = SSwapChainDesc;

        virtual void Present(uint32_t SyncInterval = 1) = 0;
        virtual const SSwapChainDesc& GetDesc()const = 0;
        virtual void Resize(uint32_t NewWidth, uint32_t NewHeight) = 0;
        virtual void SetFullscreenMode(const SDisplayModeAttribs &DisplayMode) = 0;
        virtual void SetWindowedMode() = 0;
        virtual ITextureView* GetCurrentBackBufferRTV() = 0;
        virtual ITextureView* GetDepthBufferDSV() = 0;
    };
}