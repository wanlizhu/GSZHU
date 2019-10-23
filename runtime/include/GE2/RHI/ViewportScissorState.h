#pragma once

#include "GE2/BasicTypes.h"
#include "GE2/Utilities.h"

namespace GE2::RHI
{
    struct Viewport
    {
        float x = 0.f;
        float y = 0.f;
        float width = 0.f;
        float height = 0.f;
        float minDepth = 0.f;
        float maxDepth = 0.f;
    };

    struct GE2_IMPEXP ViewportScissorState
    {
        LIST<Viewport> viewports; // will be empty if it's dynamic state
        LIST<RECT>     scissors;  // will be empty if it's dynamic state
    };
}