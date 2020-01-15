#pragma once

#include "Core/Common.h"
#include "Graphics/RHI/RHIFlags.h"
#include "Graphics/RHI/Constants.h"

namespace Wanlix
{
    struct StencilDesc
    {
        EStencilOp stencilFailOp = EStencilOp::Keep;
        EStencilOp depthFailOp = EStencilOp::Keep;
        EStencilOp stencilDepthPassOp = EStencilOp::Keep;
        EComparisonFunc stencilFunc = EComparisonFunc::Always;
    };

    struct DepthStencilStateDesc
    {
        Bool depthEnabled = true;
        Bool writeDepth = true;
        EComparisonFunc depthFunc = EComparisonFunc::Less;
        Bool stencilEnabled = false;
        Uint8 stencilReadMask = 0xff;
        Uint8 stencilWriteMask = 0xff;
        StencilDesc frontFace;
        StencilDesc backFace;
    };

    inline constexpr bool operator==(const StencilDesc& lhs, const StencilDesc& rhs)
    {
        return lhs.stencilFailOp == rhs.stencilFailOp
            && lhs.depthFailOp == rhs.depthFailOp
            && lhs.stencilDepthPassOp == rhs.stencilDepthPassOp
            && lhs.stencilFunc == rhs.stencilFunc;
    }

    inline constexpr bool operator==(const DepthStencilStateDesc& lhs, const DepthStencilStateDesc& rhs)
    {
        return lhs.depthEnabled == rhs.depthEnabled
            && lhs.writeDepth == rhs.writeDepth
            && lhs.depthFunc == rhs.depthFunc
            && lhs.stencilEnabled == rhs.stencilEnabled
            && lhs.stencilReadMask == rhs.stencilReadMask
            && lhs.stencilWriteMask == rhs.stencilWriteMask
            && lhs.frontFace == rhs.frontFace
            && lhs.backFace == rhs.backFace;
    }
}