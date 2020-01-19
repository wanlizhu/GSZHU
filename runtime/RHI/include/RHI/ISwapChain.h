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

    class ISwapChain : public IDeviceObject
    {
    public:
        virtual void Present() = 0;
        virtual void Resize(Uint width, Uint height) = 0;
        virtual void SetDisplayMode(const DisplayMode& mode) = 0;
        virtual ITextureView* GetCurrentRTV() = 0;
        virtual ITextureView* GetDSV() = 0;
    };
}