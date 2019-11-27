#include "TextureUtils.h"
#include "Wanlix/Core3D/RHI/Limits.h"

namespace Wanlix
{
    Offset ComputeTextureOffset(const TextureType type, const Offset& offset, uint32_t baseArrayLayer)
    {
        switch (type)
        {
        case TextureType::Texture1D:
            return Offset(offset.x, 0, 0);

        case TextureType::Texture1DArray:
            return Offset(offset.x, baseArrayLayer, 0);

        case TextureType::Texture2D:
        case TextureType::Texture2DMS:
            return Offset(offset.x, offset.y, 0);

        case TextureType::Texture2DArray:
        case TextureType::TextureCube:
        case TextureType::TextureCubeArray:
        case TextureType::Texture2DMSArray:
            return Offset(offset.x, offset.y, baseArrayLayer);

        case TextureType::Texture3D:
            return offset;
        }
        return {};
    }

    Extent ComputeTextureExtent(const TextureType type, const Extent& extent, uint32_t numArrayLayers)
    {
        switch (type)
        {
        case TextureType::Texture1D:
            return Extent(extent.width, 1u, 1u);

        case TextureType::Texture1DArray:
            return Extent(extent.width, numArrayLayers, 1u);

        case TextureType::Texture2D:
        case TextureType::Texture2DMS:
            return Extent(extent.width, extent.height, 1u);

        case TextureType::TextureCube:
        case TextureType::Texture2DArray:
        case TextureType::TextureCubeArray:
        case TextureType::Texture2DMSArray:
            return Extent(extent.width, extent.height, numArrayLayers);

        case TextureType::Texture3D:
            return extent;
        }
        return {};
    }

    SubresourceLayout ComputeSubresourceLayout(const Format format, const Extent& extent)
    {
        const auto& formatDesc = GetFormatAttributes(format);
        SubresourceLayout layout;
        layout.rowStride = (extent.width * formatDesc.bitSize) / formatDesc.blockWidth / 8;
        layout.layerStride = (extent.height * layout.rowStride) / formatDesc.blockHeight;
        layout.dataSize = extent.depth * layout.layerStride;
        
        return layout;
    }

    bool MustGenerateMipsOnCreate(const TextureDescriptor& textureDesc)
    {
        return NumMipLevels(textureDesc) > 1 
            && (textureDesc.miscFlags.value & (MiscFlags::GenerateMips | MiscFlags::NoInitialData)) == MiscFlags::GenerateMips;
    }

    uint32_t GetClampedSamples(uint32_t samples)
    {
        return std::clamp(samples, 1u, WANLIX_MAX_NUM_SAMPLES);
    }
}