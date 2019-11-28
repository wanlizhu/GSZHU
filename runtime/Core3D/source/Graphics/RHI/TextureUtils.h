#pragma once

#include "Wanlix/Core3D/Graphics/RHI/TextureDescriptor.h"
#include "Wanlix/Core3D/Types.h"
#include <cstdint>

namespace Wanlix
{
    struct SubresourceLayout
    {
        uint32_t rowStride = 0; // Bytes per row
        uint32_t layerStride = 0; // Bytes per layer
        uint32_t dataSize = 0; // Bytes per resource
    };


    /* ----- Functions ----- */


    Offset ComputeTextureOffset(const TextureType type, const Offset& offset, uint32_t baseArrayLayer);
    Extent ComputeTextureExtent(const TextureType type, const Extent& extent, uint32_t numArrayLayers);
    SubresourceLayout ComputeSubresourceLayout(const Format format, const Extent& extent);
    bool MustGenerateMipsOnCreate(const TextureDescriptor& textureDesc);
    uint32_t GetClampedSamples(uint32_t samples);
}