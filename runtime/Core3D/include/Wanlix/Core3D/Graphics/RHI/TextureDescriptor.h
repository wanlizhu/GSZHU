#pragma once

#include "Wanlix/Core3D/Graphics/Format.h"
#include "Wanlix/Core3D/Types.h"
#include "ResourceDescriptor.h"
#include "CommandBufferDescriptor.h"

namespace Wanlix
{
    enum class TextureType
    {
        Texture1D,          //!< 1-Dimensional texture.
        Texture2D,          //!< 2-Dimensional texture.
        Texture3D,          //!< 3-Dimensional texture.
        TextureCube,        //!< Cube texture.
        Texture1DArray,     //!< 1-Dimensional array texture.
        Texture2DArray,     //!< 2-Dimensional array texture.
        TextureCubeArray,   //!< Cube array texture.
        Texture2DMS,        //!< 2-Dimensional multi-sample texture.
        Texture2DMSArray,   //!< 2-Dimensional multi-sample array texture.
    };

    /* Can be used to change the order of texel components independently of a shader.*/
    enum class TextureSwizzle
    {
        Zero,   //!< The component is replaced by the constant zero.
        One,    //!< The component is replaced by the constant one.
        Red,    //!< The component is replaced by red component.
        Green,  //!< The component is replaced by green component.
        Blue,   //!< The component is replaced by blue component.
        Alpha   //!< The component is replaced by alpha component.
    };

    struct TextureSwizzleRGBA
    {
        TextureSwizzle r = TextureSwizzle::Red;     
        TextureSwizzle g = TextureSwizzle::Green;   
        TextureSwizzle b = TextureSwizzle::Blue;    
        TextureSwizzle a = TextureSwizzle::Alpha;   
    };

    /* Texture subresource descriptor which specifies the array layer and MIP-map level range of a texture resource.
     * The default values refer to the first array layer and the first MIP-map level.
    */
    struct TextureSubresource
    {
        uint32_t baseArrayLayer = 0; // Only used by array texture types
        uint32_t numArrayLayers = 1; // Only used by array texture types
        uint32_t baseMipLevel = 0;
        uint32_t numMipLevels = 1;
    };

    /* Texture location structure: MIP-map level and offset.
     * This is used to specifiy the source and destination location of a texture copy operation
    */
    struct TextureLocation
    {
        Offset offset;
        uint32_t arrayLayer = 0;
        uint32_t mipLevel = 0;
    };

    /* This is used to write and read the image data of a single texture MIP-map level. */
    struct TextureRegion
    {
        TextureSubresource subresource;
        Offset offset;
        Extent extent;
    };

    /* Contains all information about type, format, and dimension to create a texture resource. */
    struct TextureDescriptor
    {
        TextureType type = TextureType::Texture2D;
        BindFlags::UnderlyingType bindFlags = (BindFlags::Sampled | BindFlags::ColorAttachment);
        CPUAccessFlags::UnderlyingType cpuAccessFlags = 0;
        MiscFlags::UnderlyingType miscFlags = (MiscFlags::FixedSamples | MiscFlags::GenerateMips);
        Format format = Format::RGBA8UNorm;
        Extent extent = { 1, 1, 1 };
        uint32_t arrayLayers = 1;
        uint32_t mipLevels = 0;
        uint32_t samples = 1;
        ClearValue clearValue;
    };

    struct TextureViewDescriptor
    {
        TextureType type = TextureType::Texture2D;
        Format format = Format::RGBA8UNorm;
        TextureSubresource subresource;
        TextureSwizzleRGBA swizzle;
    };


    /* ----- Functions ----- */


    uint32_t NumMipLevels(uint32_t width, uint32_t height = 1, uint32_t depth = 1);
    uint32_t NumMipLevels(const TextureType& type, const Extent& extent);
    uint32_t NumMipLevels(const TextureDescriptor& desc);
    uint32_t NumMipDimensions(const TextureType type);
    uint32_t NumTextureDimensions(const TextureType type);
    Extent   GetMipExtent(const TextureType type, const Extent& extent, uint32_t mipLevel);
    uint32_t NumMipTexels(const TextureType type, const Extent& extent, uint32_t mipLevel);
    uint32_t NumMipTexels(const TextureType type, const Extent& extent, const TextureSubresource& subresource);
    uint32_t NumMipTexels(const TextureDescriptor& textureDesc, uint32_t mipLevel = 0);
    uint32_t TextureBufferSize(const Format format, uint32_t numTexels);

    bool IsMipMappedTexture(const TextureDescriptor& desc);
    bool IsArrayTexture(const TextureType type);
    bool IsMultiSampleTexture(const TextureType type);
    bool IsCubeTexture(const TextureType type);
    bool IsTextureSwizzleIdentity(const TextureSwizzleRGBA& swizzle);
}