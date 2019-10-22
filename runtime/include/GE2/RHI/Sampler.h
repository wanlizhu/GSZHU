#pragma once

#include "GE2/BasicTypes.h"
#include "GE2/Utilities.h"
#include "DepthStencilState.h"

namespace GE2::RHI
{
    enum class EFilter
    {
        Nearest,
        Linear,
        CubicIMG,
    };

    enum class ESamplerMipmapMode
    {
        Nearest,
        Linear,
    };

    enum class ESamplerAddressMode
    {
        Repeat,
        MirroredRepeat,
        ClampToEdge,
        ClampToBorder,
        MirrorClampToEdge,
    };

    struct GE2_IMPEXP Sampler
    {
        EFilter                magFilter = EFilter::Linear;
        EFilter                minFilter = EFilter::Linear;
        ESamplerMipmapMode     mipmapMode = ESamplerMipmapMode::Linear;
        ESamplerAddressMode    addressModeU = ESamplerAddressMode::ClampToBorder;
        ESamplerAddressMode    addressModeV = ESamplerAddressMode::ClampToBorder;
        ESamplerAddressMode    addressModeW = ESamplerAddressMode::ClampToBorder;
        float                  mipLodBias = 0.f;
        bool                   anisotropyEnable = false;
        float                  maxAnisotropy = 0.f;
        bool                   compareEnable = false;
        EComparison            compareOp = EComparison::Less;
        float                  minLod = 0.f;
        float                  maxLod = 0.f;
        std::array<uint8_t, 4> borderColor = { 0 };
        bool                   unnormalizedCoordinates = false;
    };
}