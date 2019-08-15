#pragma once

#include "Graphics/RHI/IDeviceObject.h"
#include "Graphics/RHI/DisplayAttribs.h"

namespace ZHU
{
	struct ZHU_GS_API DisplayAttribs
	{
		uint32_t Width = 0;
		uint32_t Height = 0;
		ETextureFormat Format = ETextureFormat::Unknown;
		uint32_t RefreshRateNumerator = 0;
		uint32_t RefreshRateDenominator = 0;
		EDisplayScaling Scaling = EDisplayScaling::Undefined;
		EDisplayOrder ScanOrder = EDisplayOrder::Undefined;
	};


    struct ZHU_GS_API SwapChainDesc : public IDeviceObjectDesc
    {
        uint32_t Width = 0;
        uint32_t Height = 0;
        ETextureFormat ColorFormat = ETextureFormat::RGBA8UN_SRGB;
        ETextureFormat DepthFormat = ETextureFormat::DEPTH32F;

        uint32_t SampleCount = 1;
        uint32_t BufferCount = 2;
        
        float DefaultDepthValue = 1.0f;
        uint8_t DefaultStencilValue = 0;
    };


    // The swap chain is created by a platform-dependent function
    class ZHU_GS_API ISwapChain : public Object
    {
    public:
        virtual void Present(uint32_t sync = 1) = 0;
        virtual const SwapChainDesc& GetDesc() const = 0;
        virtual void Resize(uint32_t width, uint32_t height) = 0;
        virtual void SetFullscreenMode(const DisplayAttribs& display) = 0;
        virtual void SetWindowedMode() = 0;
        virtual IDeviceObject::Pointer GetCurrentRTV() const = 0;
        virtual IDeviceObject::Pointer GetDSV() const = 0;
    };
}