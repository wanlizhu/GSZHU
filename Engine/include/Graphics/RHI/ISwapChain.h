#pragma once

#include "Graphics/IDeviceObject.h"
#include "Graphics/DisplayAttributes.h"

namespace ZHU
{
    struct SwapChainDesc : public DeviceObjectDesc
    {
        uint32_t Width = 0;
        uint32_t Height = 0;
        EPixelFormat ColorFormat = EPixelFormat::RGBA8_SRGB;
        EPixelFormat DepthFormat = EPixelFormat::DEPTH32F;

        uint32_t SampleCount = 1;
        uint32_t BufferCount = 2;
        
        float DefaultDepthValue = 1.0f;
        uint8_t DefaultStencilValue = 0;
    };


    // The swap chain is created by a platform-dependent function
    class ZHU_API ISwapChain : public Object
    {
    public:
        virtual void Present(uint32_t sync = 1) = 0;
        virtual const SwapChainDesc& GetDesc() const = 0;
        virtual void Resize(uint32_t width, uint32_t height) = 0;
        virtual void SetFullscreenMode(const DisplayAttributes& display) = 0;
        virtual void SetWindowedMode() = 0;
        virtual IDeviceObject::Pointer GetCurrentRTV() const = 0;
        virtual IDeviceObject::Pointer GetDSV() const = 0;
    };
}