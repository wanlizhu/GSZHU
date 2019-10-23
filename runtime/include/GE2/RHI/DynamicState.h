#pragma once

#include "GE2/BasicTypes.h"
#include "GE2/Utilities.h"
#include "ForwardDeclarations.h"

namespace GE2::RHI
{
    enum class EDynamicState
    {
        Viewport = 0,
        Scissor = 1,
        LineWidth = 2,
        DepthBias = 3,
        BlendConstants = 4,
        DepthBounds = 5,
        StencilCompareMask = 6,
        StencilWriteMask = 7,
        StencilReference = 8,
    };

    struct GE2_IMPEXP DynamicState
    {
        // mark specified states as dynamic, 
        // so that user can set them without changing the pipeline
        LIST<EDynamicState> states;
    };
}