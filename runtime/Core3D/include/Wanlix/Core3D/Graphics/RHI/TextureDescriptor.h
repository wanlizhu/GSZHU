#pragma once

#include "Wanlix/Core3D/Graphics/Types.h"
#include "ResourceDescriptor.h"

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

    struct TextureDescriptor : public ResourceDescriptor
    {
        TextureType type = TextureType::Texture2D;
        Format      format = Format::RGBA8UNorm;

        Extent      extent = { 1, 1, 1 };
        uint32_t    arrayLayers = 1;
        uint32_t    mipLevels = 0;
        uint32_t    samples = 1;
        ClearValue  clearValue;

        TextureDescriptor()
            : ResourceDescriptor(ResourceType::Texture)
        {}
    };

    struct TextureViewDescriptor
    {
        TextureType type = TextureType::Texture2D;
        Format      format = Format::RGBA8UNorm;
    };
}