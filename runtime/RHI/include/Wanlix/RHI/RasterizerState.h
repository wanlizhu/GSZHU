#pragma once

#include "Common.h"

namespace Wanlix
{
    struct RasterizerStateDesc
    {
        FillMode fillMode = FillMode::Solid;
        CullMode cullMode = CullMode::Back;
        Bool frontCCw = true;
        Bool depthClipEnabled = true;
        Bool scissorEnabled = false;
        Bool antialiasedLineEnabled = false;
        Int depthBias = 0;
        float depthBiasClamp = 0.f;
        float slopeScaledDepthBias = 0.f;
    };
}