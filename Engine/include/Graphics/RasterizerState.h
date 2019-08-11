#pragma once

#include "Graphics/Constants.h"

namespace ZHU
{
    struct RasterizerState
    {
        EFillMode FillMode = EFillMode::Solid;
        ECullMode CullMode = ECullMode::Back;
        bool IsFrontCCW = true;
        bool EnableDepthClip = true;
        bool EnableScissor = false;
        bool EnableAntialiasedLine = false;
        int DepthBias = 0;
        float DepthBiasClamp = 0.0f;
        float SlopeScaledDepthBias = 0.0f;
    };
}