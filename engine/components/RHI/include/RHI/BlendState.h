#pragma once

#include "Wanlix/Common.h"
#include "RHI/Flags.h"
#include "RHI/Constants.h"

namespace Wanlix
{
    struct BlendFunc
    {
        EBlendFactor src;
        EBlendFactor dst;
        EBlendOp op;
    };

    struct RenderTargetBlendDesc
    {
        Bool blendEnabled = false;
        Bool logicalOpEnabled = false;
        ELogicOp logicalOp;
        Uint8 writeMask = 0;
        BlendFunc colorBlend;
        BlendFunc alphaBlend;
    };

    struct BlendStateDesc
    {
        Bool alphaToCoverageEnabled = false;
        Bool independentBlendEnabled = false;
        Float4 blendFactor = Float4(0, 0, 0, 0);
        Array<RenderTargetBlendDesc> renderTargets;
    };
}