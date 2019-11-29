#include "Wanlix/Core3D/Graphics/RHI/Texture.h"
#include "Wanlix/Core3D/Graphics/Config.h"
#include <cmath>

namespace Wanlix
{
    Texture::Texture(StringCRef name) noexcept
        : Resource(name)
    {}

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
            && (textureDesc.miscFlags & (MiscFlags::GenerateMips | MiscFlags::NoInitialData)) == MiscFlags::GenerateMips;
    }

    uint32_t GetClampedSamples(uint32_t samples)
    {
        return std::clamp(samples, 1u, WANLIX_MAX_NUM_SAMPLES);
    }

    uint32_t NumMipLevels(uint32_t width, uint32_t height, uint32_t depth)
    {
        const auto maxSize = std::max({ width, height, depth });
        const auto log2Size = static_cast<std::uint32_t>(std::log2(maxSize));
        return (1u + log2Size);
    }

    uint32_t NumMipLevels(const TextureType& type, const Extent& extent)
    {
        switch (type)
        {
        case TextureType::Texture1D:        return NumMipLevels(extent.width);
        case TextureType::Texture2D:        return NumMipLevels(extent.width, extent.height);
        case TextureType::Texture3D:        return NumMipLevels(extent.width, extent.height, extent.depth);
        case TextureType::TextureCube:      return NumMipLevels(extent.width, extent.height);
        case TextureType::Texture1DArray:   return NumMipLevels(extent.width);
        case TextureType::Texture2DArray:   return NumMipLevels(extent.width, extent.height);
        case TextureType::TextureCubeArray: return NumMipLevels(extent.width, extent.height);
        case TextureType::Texture2DMS:      return 1u;
        case TextureType::Texture2DMSArray: return 1u;
        }
        return 0u;
    }

    uint32_t NumMipLevels(const TextureDescriptor& desc)
    {
        if (desc.mipLevels == 0)
            return NumMipLevels(desc.type, desc.extent);
        else
            return desc.mipLevels;
    }

    uint32_t NumMipDimensions(const TextureType type)
    {
        switch (type)
        {
        case TextureType::Texture1D:        return 1;
        case TextureType::Texture2D:        return 2;
        case TextureType::Texture3D:        return 3;
        case TextureType::TextureCube:      return 2;
        case TextureType::Texture1DArray:   return 2; // Array layer +1 dimension
        case TextureType::Texture2DArray:   return 3; // Array layer +1 dimension
        case TextureType::TextureCubeArray: return 3; // Array layer +1 dimension
        case TextureType::Texture2DMS:      return 2;
        case TextureType::Texture2DMSArray: return 3; // Array layer +1 dimension
        }
        return 0;
    }

    uint32_t NumTextureDimensions(const TextureType type)
    {
        switch (type)
        {
        case TextureType::Texture1D:        return 1;
        case TextureType::Texture2D:        return 2;
        case TextureType::Texture3D:        return 3;
        case TextureType::TextureCube:      return 2;
        case TextureType::Texture1DArray:   return 1;
        case TextureType::Texture2DArray:   return 2;
        case TextureType::TextureCubeArray: return 2;
        case TextureType::Texture2DMS:      return 2;
        case TextureType::Texture2DMSArray: return 2;
        }
        return 0;
    }
    // Returns the 1D extent for the specified MIP-map
    static std::uint32_t MipExtent(std::uint32_t extent, std::uint32_t mipLevel)
    {
        return std::max(1u, extent >> mipLevel);
    }

    Extent GetMipExtent(const TextureType type, const Extent& e, uint32_t m)
    {
        if (m < NumMipLevels(type, e))
        {
            switch (type)
            {
            case TextureType::Texture1D:        return Extent{ MipExtent(e.width, m), 1u, 1u };
            case TextureType::Texture2D:        return Extent{ MipExtent(e.width, m), MipExtent(e.height, m), 1u };
            case TextureType::Texture3D:        return Extent{ MipExtent(e.width, m), MipExtent(e.height, m), MipExtent(e.depth, m) };
            case TextureType::TextureCube:      return Extent{ MipExtent(e.width, m), MipExtent(e.height, m), 1u };
            case TextureType::Texture1DArray:   return Extent{ MipExtent(e.width, m), e.height, 1u };
            case TextureType::Texture2DArray:   return Extent{ MipExtent(e.width, m), MipExtent(e.height, m), e.depth };
            case TextureType::TextureCubeArray: return Extent{ MipExtent(e.width, m), MipExtent(e.height, m), e.depth };
            case TextureType::Texture2DMS:      return Extent{ e.width, e.height, 1u };
            case TextureType::Texture2DMSArray: return Extent{ e.width, e.height, e.depth };
            }
        }
        return {};
    }

    uint32_t NumMipTexels(const TextureType type, const Extent& extent, uint32_t mipLevel)
    {
        auto mipExtent = GetMipExtent(type, extent, mipLevel);
        return (mipExtent.width * mipExtent.height * mipExtent.depth);
    }

    uint32_t NumMipTexels(const TextureType type, const Extent& extent, const TextureSubresource& subresource)
    {
        std::uint32_t numTexels = 0;

        const auto subresourceExtent = ComputeTextureExtent(type, extent, subresource.numArrayLayers);
        for (std::uint32_t mipLevel = 0; mipLevel < subresource.numMipLevels; ++mipLevel)
            numTexels += NumMipTexels(type, subresourceExtent, subresource.baseMipLevel + mipLevel);

        return numTexels;
    }

    uint32_t NumMipTexels(const TextureDescriptor& textureDesc, uint32_t mipLevel)
    {
        const auto extent = ComputeTextureExtent(textureDesc.type, textureDesc.extent, textureDesc.arrayLayers);

        if (mipLevel == ~0u)
        {
            std::uint32_t numTexels = 0;

            const auto numMipLevels = NumMipLevels(textureDesc);
            for (mipLevel = 0; mipLevel < numMipLevels; ++mipLevel)
                numTexels += NumMipTexels(textureDesc.type, extent, mipLevel);

            return numTexels;
        }

        return NumMipTexels(textureDesc.type, extent, mipLevel);
    }

    uint32_t TextureBufferSize(const Format format, uint32_t numTexels)
    {
        const auto& formatDesc = GetFormatAttributes(format);
        const auto blockSize = formatDesc.blockWidth * formatDesc.blockHeight;
        if (blockSize > 0 && numTexels % blockSize == 0)
            return ((numTexels / blockSize * formatDesc.bitSize) / 8);
        else
            return 0;
    }

    bool IsMipMappedTexture(const TextureDescriptor& desc)
    {
        return (!IsMultiSampleTexture(desc.type) && (desc.mipLevels == 0 || desc.mipLevels > 1));
    }

    bool IsArrayTexture(const TextureType type)
    {
        return (type >= TextureType::Texture1DArray && type != TextureType::Texture2DMS);
    }

    bool IsMultiSampleTexture(const TextureType type)
    {
        return (type >= TextureType::Texture2DMS);
    }

    bool IsCubeTexture(const TextureType type)
    {
        return (type == TextureType::TextureCube || type == TextureType::TextureCubeArray);
    }

    bool IsTextureSwizzleIdentity(const TextureSwizzleRGBA& swizzle)
    {
        return swizzle.r == TextureSwizzle::Red 
            && swizzle.g == TextureSwizzle::Green 
            && swizzle.b == TextureSwizzle::Blue 
            && swizzle.a == TextureSwizzle::Alpha;
    }
}