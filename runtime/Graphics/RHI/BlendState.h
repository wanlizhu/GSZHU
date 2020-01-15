#pragma once

#include "Core/Common.h"
#include "Graphics/RHI/RHIFlags.h"
#include "Graphics/RHI/Constants.h"

namespace Wanlix
{
    struct BlendFunc
    {
        EBlendFactor src;
        EBlendFactor dst;
        EBlendOp op;
    };

    struct RenderTargetBlendDesc
    {
        Bool blendEnabled = false;
        Bool logicalOpEnabled = false;
        ELogicOp logicalOp;
        Uint8 writeMask = 0;
        BlendFunc colorBlend;
        BlendFunc alphaBlend;
    };

    struct BlendStateDesc
    {
        Bool alphaToCoverageEnabled = false;
        Bool independentBlendEnabled = false;
        Float4 blendFactor = Float4(0, 0, 0, 0);
        RenderTargetBlendDesc renderTargets[MAX_NUM_RENDER_TARGET] = {};
    };

    inline constexpr bool operator==(const BlendFunc& lhs, const BlendFunc& rhs)
    {
        return lhs.src == rhs.src
            && lhs.dst == rhs.dst
            && lhs.op == rhs.op;
    }

    inline constexpr bool operator==(const RenderTargetBlendDesc& lhs, const RenderTargetBlendDesc& rhs)
    {
        return lhs.blendEnabled == rhs.blendEnabled
            && lhs.logicalOpEnabled == rhs.logicalOpEnabled
            && lhs.logicalOp == rhs.logicalOp
            && lhs.writeMask == rhs.writeMask
            && lhs.colorBlend == rhs.colorBlend
            && lhs.alphaBlend == rhs.alphaBlend;
    }

    inline constexpr bool operator==(const BlendStateDesc& lhs, const BlendStateDesc& rhs)
    {
        return lhs.alphaToCoverageEnabled == rhs.alphaToCoverageEnabled
            && lhs.independentBlendEnabled == rhs.independentBlendEnabled
            && lhs.blendFactor == rhs.blendFactor
            && ArrayEquals<RenderTargetBlendDesc>(lhs.renderTargets, rhs.renderTargets, MAX_NUM_RENDER_TARGET);
    }
}