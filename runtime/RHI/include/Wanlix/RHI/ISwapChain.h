#pragma once

#include "Common.h"
#include "ITextureView.h"

namespace Wanlix
{
    struct DisplayAttribs
    {
        Uint width = 0;
        Uint height = 0;
        PixelFormat format = PixelFormat::Undefined;
        Uint refreshRateNumerator = 0;
        Uint refreshRateDenominator = 0;
        ScalingMode scaling = ScalingMode::Unspecified;
        ScanLineMode scanLine = ScanLineMode::Unspecified;
    };

    struct SwapChainDesc
    {
        Uint width = 0;
        Uint height = 0;
        PixelFormat colorAttachmentFormat;
        PixelFormat depthStencilAttachmentFormat;
        Uint usage = SwapChainUsage::RenderTarget;
        Uint samples = 1;
        Uint buffers = 2;
        float defaultDepth = 1.f;
        Uint defaultStencil = 0;
        Bool isPrimary = true;
    }; 

    class RHI_API ISwapChain : public Object
    {
        CLASSINFO(ISwapChain)
        INHERIT_SHARED_FROM_THIS(Object)
    public:
        virtual void Present(Uint syncInterval = 1) = 0;
        virtual SwapChainDesc const& GetDesc() const = 0;
        virtual void Resize(Uint w, Uint h) = 0;
        virtual void SetFullScreenMode(DisplayAttribs const& attribs) = 0;
        virtual void SetWindowedMode() = 0;
        virtual ITextureView* GetCurrentFrame() = 0;
        virtual ITextureView* GetDepthStencil() = 0;
    };
}