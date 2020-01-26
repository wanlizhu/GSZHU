#pragma once

#include "Wanlix/Common.h"
#include "RHI/Flags.h"
#include "RHI/Constants.h"

namespace Wanlix
{
    struct RasterizerStateDesc
    {
        EFillMode fillMode = EFillMode::Solid;
        ECullMode cullMode = ECullMode::Back;
        Bool frontCCW = true;
        Bool depthClipEnabled = true;
        Bool scissorEnabled = false;
        Bool lineAAEnabled = false;
        int depthBias = 0;
        float depthBiasClamp = 0.f;
        float slopeScaledDepthBias = 0.f;
    };
}