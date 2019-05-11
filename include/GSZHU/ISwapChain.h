#pragma once

#include <GSZHU/IObject.h>
#include <GSZHU/ITextureView.h>
#include <GSZHU/DESC/SSwapChainDesc.h>
#include <GSZHU/SDisplayModeAttribs.h>

namespace GSZHU {
    // The swap chain is created by a platform-dependent function
    class ISwapChain : public IObject {
    public:
        virtual void Present(UINT SyncInterval = 1) = 0;
        virtual const SSwapChainDesc& GetDesc()const = 0;
        virtual void Resize(UINT NewWidth, UINT NewHeight) = 0;
        virtual void SetFullscreenMode(const SDisplayModeAttribs &DisplayMode) = 0;
        virtual void SetWindowedMode() = 0;
        virtual ITextureView* GetCurrentBackBufferRTV() = 0;
        virtual ITextureView* GetDepthBufferDSV() = 0;
    };
}