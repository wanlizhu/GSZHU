#pragma once

#include "Core/Common.h"
#include "RHI/Flags.h"
#include "RHI/Constants.h"

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
}