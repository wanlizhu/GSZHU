#pragma once

#include "Graphics/ISampler.h"
#include "Graphics/ClearValue.h"
#include "Graphics/ITextureView.h"

namespace ZHU
{
    struct TextureDescriptor : public DeviceObjectDescriptor
    {
        ETextureDimension Type = ETextureDimension::Undefined;
        uint32_t Width = 0;
        uint32_t Height = 0;
        union {
            uint32_t ArraySize = 1; // For a 1D/2D texture array
            uint32_t Depth; // For a 3D texture
        };
        
        EPixelFormat Format = EPixelFormat::Unknown;
        uint32_t MipLevels = 1; // Multisampled textures can only have 1 Mip level.
        uint32_t SampleCount = 1; // Only 2D textures or 2D texture arrays can be multisampled.

        EUsage Usage = EUsage::Default;
        BitSet<EBind> BindFlags = conv(EBind::None);
        BitSet<EMappedCPUAccess> MappedCPUAccess = conv(EMappedCPUAccess::None);
        OptimizedClearValue ClearValue;
        uint64_t CommandQueueMask = 1;
    };


    class ZHU_API ITexture : public IDeviceObject
    {
    public:
        virtual const TextureDescriptor& GetDesc() const = 0;
        // Texture view will contain strong reference to the texture
        virtual IDeviceObject::Pointer CreateView(const TextureViewDescriptor& viewDesc) = 0;
        virtual IDeviceObject::Pointer GetDefaultView(ETextureView type) const = 0;
        virtual void* GetNativeHandle() const = 0;
        virtual void SetState(EResourceState state) = 0;
        virtual EResourceState GetState() const = 0;
    };
}