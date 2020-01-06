#pragma once

#include "Wanlix/Common.h"
#include "Wanlix/ConfigRHI.h"
#include "Wanlix/FlagsRHI.h"
#include "Wanlix/Format.h"
#include "Wanlix/Color.h"

namespace Wanlix
{
    using BlendStateHandle = void*;
    using SamplerHandle = void*;
    using ResourceHandle = void*;
    using ResourceViewHandle = void*;

    struct Offset
    {
        Uint x = 0;
        Uint y = 0;
        Uint z = 0;
    };

    struct Extent
    {
        Uint width = 0;
        Uint height = 0;
        Uint depth = 1;
    };

    struct Region
    {
        Offset offset;
        Extent extent;
    };

    struct Subresource
    {
        Uint baseArrayLayer = 0;
        Uint numArrayLayers = 1;
        Uint baseMipLevel = 0;
        Uint numMipLevels = 1;

        Subresource() = default;
        Bool IsGlobal() const {
            static Subresource global;
            return *this == global;
        }
        Bool operator==(const Subresource& rhs) const {
            return baseArrayLayer == rhs.baseArrayLayer
                && numArrayLayers == rhs.numArrayLayers
                && baseMipLevel == rhs.baseMipLevel
                && numMipLevels == rhs.numMipLevels;
        }
    };

    struct Viewport
    {
        float xTL = 0.f;
        float yTL = 0.f;
        float width = 0.f;
        float height = 0.f;
        float minDepth = 0.f;
        float maxDepth = 1.f;
    };

    struct ClearValue
    {
        PixelFormat format = PixelFormat::Undefined;
        Color color;
        float depth = 0.f;
        Uint stencil = 0;
    };

    struct ResourceStateDesc
    {
        Bool isGlobal = true;
        ResourceState globalState = ResourceState::Undefined;
        Array<ResourceState> subresources;
    };
}

namespace std
{
    template<>
    struct hash<Wanlix::Subresource>
    {
        std::size_t operator()(Wanlix::Subresource const& key) const
        {
            return ((std::hash<uint32_t>()(key.baseArrayLayer)
                    ^ (std::hash<uint32_t>()(key.numArrayLayers) << 1)) >> 1)
                    ^ (std::hash<uint32_t>()(key.baseMipLevel) << 1)
                    ^ (std::hash<uint32_t>()(key.numMipLevels) << 3);
        }
    };
}