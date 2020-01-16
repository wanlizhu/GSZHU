#pragma once

#include "Graphics/RHI/ITextureView.h"

namespace Wanlix
{
    struct DisplayMode
    {
        Uint width = 0;
        Uint height = 0;
        EPixelFormat pixelFormat = EPixelFormat::Undefined;
        Uint refreshRate = 0;
        EScaling scaling = EScaling::Unspecified;
        EScanLineOrder scanLineOrder = EScanLineOrder::Unspecified;
    };

    struct SwapChainDesc : public DeviceObjectDesc
    {};

    class ISwapChain : public IDeviceObject
    {
    public:
        virtual void Present() = 0;
        virtual void Resize(Uint width, Uint height) = 0;
        virtual void SetDisplayMode() = 0;
        virtual ITextureView* GetCurrentRTV() = 0;
        virtual ITextureView* GetDSV() = 0;

        // Interfaces inherited from IDeviceObject
        virtual void SetName(const String& name) = 0;
        virtual const String& GetName() const = 0;
        virtual Handle ApiHandle() const = 0;
        virtual const SwapChainDesc& GetDesc() const = 0;
    };
}