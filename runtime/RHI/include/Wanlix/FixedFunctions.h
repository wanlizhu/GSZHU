#pragma once

#include "Wanlix/CommonRHI.h"

namespace Wanlix
{
    struct BlendState
    {
        struct RenderTargetDesc
        {
            Bool blendEnabled = false;
            struct {
                Bool red = true;
                Bool green = true;
                Bool blue = true;
                Bool alpha = true;
            } writeMask;

            BlendOp rgbBlendOp = BlendOp::Add;
            BlendFactor srcRgbFactor = BlendFactor::One;
            BlendFactor dstRgbFactor = BlendFactor::Zero;

            BlendOp alphaBlendOp = BlendOp::Add;
            BlendFactor srcAlphaFactor = BlendFactor::One;
            BlendFactor dstAlphaFactor = BlendFactor::Zero;
        };

        Bool enableIndependentBlend = false;
        Bool alphaToCoverageEnabled = false;
        Float4 blendFactor = { 0.f, 0.f, 0.f, 0.f };
        Array<RenderTargetDesc> renderTargets;
    };

    struct BlendStateObject : public BlendState
    {
        BlendStateHandle apiHandle = nullptr;
    };
}