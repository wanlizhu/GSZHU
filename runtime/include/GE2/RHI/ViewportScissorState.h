#pragma once

#include "GE2/BasicTypes.h"
#include "GE2/Utilities.h"

namespace GE2::RHI
{
    struct GE2_IMPEXP Viewport
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
        std::vector<Viewport> viewports; // will be empty if it's dynamic state
        std::vector<Rect2D>   scissors;  // will be empty if it's dynamic state
    };
}