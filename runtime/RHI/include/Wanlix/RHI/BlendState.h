#pragma once

#include "Common.h"

namespace Wanlix
{
    struct RenderTargetBlendDesc
    {
        Bool blendEnabled = false;
        Bool logicOpEnabled = false;

        BlendFactor colorSrcFactor = BlendFactor::One;
        BlendFactor colorDestFactor = BlendFactor::Zero;
        BlendOp     colorBlendOp = BlendOp::Add;

        BlendFactor alphaSrcFactor = BlendFactor::One;
        BlendFactor alphaDestFactor = BlendFactor::Zero;
        BlendOp     alphaBlendOp = BlendOp::Add;

        LogicOp logicOp = LogicOp::NoOp;
        Byte rendertargetWriteMask = ColorMask::All;
    };

    struct BlendStateDesc
    {
        Bool alphaToCoverageEnabled = false;
        Bool independentBlendEnabled = false;
        RenderTargetBlendDesc renderTargets[RHI_MAX_NUM_COLOR_ATTACHMENTS];
    };
}