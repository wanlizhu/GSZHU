#pragma once

#include "Wanlix/Core3D/Graphics/Types.h"
#include "Resource.h"

namespace Wanlix
{
    class Texture : public Resource
    {
    public:
        using Ptr      = std::shared_ptr<Texture>;
        using ConstPtr = std::shared_ptr<const Texture>;
        using Desc     = TextureDescriptor;

        virtual TextureType GetTextureType() const = 0;
        virtual const Desc& GetDescriptor() const = 0;
        virtual Extent      GetMipExtent(uint32_t mipLevel) const = 0;

    protected:
        Texture(StringCRef name) noexcept;
    };


    /* ----- Functions ----- */


    uint32_t NumMipLevels(uint32_t width, uint32_t height = 1, uint32_t depth = 1);
    uint32_t NumMipLevels(const TextureType& type, const Extent& extent);
    uint32_t NumMipLevels(const TextureDescriptor& desc);

    uint32_t NumMipDimensions(const TextureType type);
    uint32_t NumTextureDimensions(const TextureType type);

    uint32_t TextureBufferSize(const Format format, uint32_t numTexels);
    Extent   GetMipExtent(const TextureType type, const Extent& extent, uint32_t mipLevel);

    bool IsMipMappedTexture(const TextureDescriptor& desc);
    bool IsArrayTexture(const TextureType type);
    bool IsMultiSampleTexture(const TextureType type);
    bool IsCubeTexture(const TextureType type);
    bool IsTextureSwizzleIdentity(const TextureSwizzleRGBA& swizzle);

    uint32_t NumMipTexels(const TextureType type, const Extent& extent, uint32_t mipLevel);
    uint32_t NumMipTexels(const TextureType type, const Extent& extent, const TextureSubresource& subresource);
    uint32_t NumMipTexels(const TextureDescriptor& textureDesc, uint32_t mipLevel = 0);
}