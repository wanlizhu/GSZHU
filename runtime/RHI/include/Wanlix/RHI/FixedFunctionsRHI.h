#pragma once

#include "Wanlix/RHI/CommonRHI.h"

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

            EBlendOp rgbBlendOp = EBlendOp::Add;
            EBlendFactor srcRgbFactor = EBlendFactor::One;
            EBlendFactor dstRgbFactor = EBlendFactor::Zero;

            EBlendOp alphaBlendOp = EBlendOp::Add;
            EBlendFactor srcAlphaFactor = EBlendFactor::One;
            EBlendFactor dstAlphaFactor = EBlendFactor::Zero;
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

    struct Sampler
    {
        EFilterType magFilter = EFilterType::Point;
        EFilterType minFilter = EFilterType::Point;
        EFilterType mipFilter = EFilterType::Point;
        Uint maxAnisotropy = 1;
        float maxLod = 1000.f;
        float minLod = -1000.f;
        float lodBias = 0.f;
        EComparisonFunc comparisonFunc = EComparisonFunc::Always;
        EAddressMode addressU = EAddressMode::Wrap;
        EAddressMode addressV = EAddressMode::Wrap;
        EAddressMode addressW = EAddressMode::Wrap;
        Float4 borderColor = { 0.f, 0.f, 0.f, 0.f };
    };

    struct SamplerObject : public Sampler
    {
        SamplerHandle apiHandle = nullptr;
    };
}