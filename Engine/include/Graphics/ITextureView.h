#pragma once

#include "Graphics/Constants.h"
#include "Graphics/DeviceObjectDesc.h"
#include "Graphics/IDeviceObject.h"
#include "Graphics/ISampler.h"
#include "Graphics/ITexture.h"

namespace ZHU
{
    struct TextureViewDesc : public DeviceObjectDesc
    {
        ETextureView Type = ETextureView::Undefined;
        ETextureDimension Dimension = ETextureDimension::Undefined;
        EPixelFormat Format = EPixelFormat::Unknown;
        
        uint32_t MostDetailedMip = 0;
        uint32_t NumMipLevels = 0;
        union {
            uint32_t FirstArraySlice = 0; // For a texture array, first array slice to address in the view
            uint32_t FirstDepthSlice;     // For a 3D texture, first depth slice to address the view
        };
        union {
            uint32_t NumArraySlices = 0; // For a texture array
            uint32_t NumDepthSlices; // For a 3D texture
        };

        EUAVAccess UAVAccess = EUAVAccess::Undefined;
    };


    class ZHU_API ITextureView : public IDeviceObject
    {
    public:
        virtual const TextureViewDesc& GetDesc() const = 0;
        virtual void SetSampler(ISampler::Pointer sampler) = 0;
        virtual ISampler::Pointer GetSampler() const = 0;
        virtual ITexture::Pointer GetTexture() const = 0;
    };
}