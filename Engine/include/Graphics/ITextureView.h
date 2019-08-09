#pragma once

#include "Graphics/Constants.h"
#include "Graphics/DeviceObjectDescriptor.h"

namespace ZHU
{
    struct TextureViewDescriptor : public DeviceObjectDescriptor
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
}