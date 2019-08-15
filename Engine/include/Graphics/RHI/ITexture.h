#pragma once

#include "Graphics/RHI/ISampler.h"
#include "Graphics/RHI/ITextureView.h"
#include "Graphics/RHI/IDeviceResource.h"

namespace ZHU
{
    struct ZHU_GS_API TextureDesc : public IDeviceResourceDesc
    {
		struct ZHU_GS_API OptimizedClearValue
		{
			ETextureFormat Format = ETextureFormat::Unknown;
			float Color[4] = { 0.0f, 0.0f, 0.0f, 0.0f };
			float Depth = 1.0f;
			uint8_t Stencil = 0;
		};

		EResourceDimension Type = EResourceDimension::Undefined;
        uint32_t Width = 0;
        uint32_t Height = 0;
        union {
            uint32_t ArraySize = 1; // For a 1D/2D texture array
            uint32_t Depth; // For a 3D texture
        };
        
        ETextureFormat Format = ETextureFormat::Unknown;
        uint32_t MipLevels = 1; // Multisampled textures can only have 1 Mip level.
        uint32_t SampleCount = 1; // Only 2D textures or 2D texture arrays can be multisampled.

        EUsage Usage = EUsage::Default;
        EBindFlags BindFlags = EBindFlags::None;
        EAccess CPUAccess = EAccess::None;
        OptimizedClearValue ClearValue;
        uint64_t CommandQueueMask = 1;
    };


    class ZHU_GS_API ITexture : public IDeviceResource
    {
    public:
        virtual const TextureDesc& GetDesc() const = 0;
        // Texture view will contain strong reference to the texture
        virtual IDeviceObject::Pointer CreateView(const TextureViewDesc& viewDesc) = 0;
        virtual IDeviceObject::Pointer GetDefaultView(ETextureView type) const = 0;
        virtual void* GetNativeHandle() const = 0;
        virtual void SetState(EResourceState state) = 0;
        virtual EResourceState GetState() const = 0;
    };
}