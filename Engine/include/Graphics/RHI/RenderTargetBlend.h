#pragma once

#include "Graphics/Constants.h"
#include "Graphics/Tools.h"

namespace ZHU
{
    struct BlendEquation
    {
        EBlendFactor Source = EBlendFactor::One;
        EBlendFactor Destination = EBlendFactor::Zero;
        EBlendOperation Operation = EBlendOperation::Add;
    };

    struct RenderTargetBlend
    {
        bool EnableBlend = false;
        bool EnableLogicOperation = false;
        BitSet<EColorMask> RenderTargetWriteMask = conv(EColorMask::All);

        BlendEquation EquationRGB;
        BlendEquation EquationAlpha;
    };
}