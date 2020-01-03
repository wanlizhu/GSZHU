#pragma once

#include "Common.h"

namespace Wanlix
{
    struct StencilFaceDesc
    {
        StencilOp stencilFail = StencilOp::Keep;
        StencilOp depthFail = StencilOp::Keep;
        StencilOp stencilDepthPass = StencilOp::Replace;
        ComparisonFunc stencilFunc = ComparisonFunc::Equal;
    };

    struct DepthStencilStateDesc
    {
        Bool depthEnabled = true;
        Bool depthWriteEnabled = true;
        ComparisonFunc depthFunc = ComparisonFunc::Less;

        Bool stencilEnabled = false;
        Byte stencilReadMask = 0xff;
        Byte stencilWriteMask = 0xff;
        StencilFaceDesc frontFace;
        StencilFaceDesc backFace;
    };
}