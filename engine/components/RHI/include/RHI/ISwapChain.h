#pragma once

#include "RHI/ITextureView.h"

namespace Wanlix
{
    struct DisplayMode
    {
        Bool fullscreen = false;
        Uint width = 0;
        Uint height = 0;
        EPixelFormat pixelFormat = EPixelFormat::Undefined;
        Uint refreshRate = 0;
        EScaling scaling = EScaling::Unspecified;
        EScanLineOrder scanLineOrder = EScanLineOrder::Unspecified;
    };

    struct SwapChainDesc : public DeviceObjectDesc 
    {
        Uint width = 0;
        Uint height = 0;
        EPixelFormat colorFormat = EPixelFormat::RGBA8UNorm_sRGB;
        EPixelFormat depthFormat = EPixelFormat::D32Float;
        ESwapChainUsage usage = ESwapChainUsage::RenderTarget;
        Uint bufferCount = 2;
        float defaultDepth = 1.f;
        Uint8 defaultStencil = 0;
        Bool isPrimary = true;
    };

    class ISwapChain : public IDeviceObject
    {
    public:
        using Desc = SwapChainDesc;

        virtual void Present() = 0;
        virtual void Resize(Uint width, Uint height) = 0;
        virtual void SetDisplayMode(const DisplayMode& mode) = 0;
        virtual ITextureView* GetCurrentRTV() = 0;
        virtual ITextureView* GetDSV() = 0;
    };
}