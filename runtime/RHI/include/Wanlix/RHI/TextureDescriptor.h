#pragma once

#include "Common.h"
#include "CommandBufferDescriptor.h"
#include "ResourceDescriptor.h"

namespace Wanlix
{
    enum class TextureType
    {
        Texture1D,          
        Texture2D,          
        Texture3D,          
        TextureCube,        
        Texture1DArray,     
        Texture2DArray,     
        TextureCubeArray,   
        Texture2DMS,        //!< 2-Dimensional multi-sample texture.
        Texture2DMSArray,   //!< 2-Dimensional multi-sample array texture.
    };

    struct TextureDescriptor : public Resource::Desc
    {
        TextureType    type = TextureType::Texture2D;
        ResourceFormat format = ResourceFormat::RGBA8UNorm;

        Extent      extent = { 1, 1, 1 };
        uint32_t    arrayLayers = 1;
        uint32_t    mipLevels = 0;
        uint32_t    samples = 1;
    };

    struct TextureViewDescriptor : public ResourceViewDescriptor
    {
        TextureType    type = TextureType::Texture2D;
        ResourceFormat format = ResourceFormat::RGBA8UNorm;
        Subresource    subresource;
        SwizzleRGBA    swizzle;
    };
}