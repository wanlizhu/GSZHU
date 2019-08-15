#pragma once

#include "Graphics/RHI/Constants.h"
#include "Graphics/RHI/DeviceObjectDesc.h"
#include "Graphics/RHI/IDeviceObject.h"
#include "Graphics/RHI/ISampler.h"
#include "Graphics/RHI/ITexture.h"

namespace ZHU
{
    struct ZHU_GS_API TextureViewDesc : public DeviceObjectDesc
    {
        ETextureView Type = ETextureView::Undefined;
        ETextureDimension Dimension = ETextureDimension::Undefined;
        ETextureFormat Format = ETextureFormat::Unknown;
        
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

        EUAVAccess UAVAccess = EUAVAccess::None;
    };


    class ZHU_GS_API ITextureView : public IDeviceObject
    {
    public:
        virtual const TextureViewDesc& GetDesc() const = 0;
        virtual void SetSampler(ISampler::Pointer sampler) = 0;
        virtual ISampler::Pointer GetSampler() const = 0;
        virtual ITexture::Pointer GetTexture() const = 0;
    };
}