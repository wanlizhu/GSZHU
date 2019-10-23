#pragma once

#include "GE2/BasicTypes.h"
#include "GE2/Utilities.h"
#include "ForwardDeclarations.h"

namespace GE2::RHI
{
    struct GE2_IMPEXP MultisampleState
    {
        uint8_t  rasterizationSamples = 1;
        bool     sampleShadingEnable = false;
        float    sampleShadingMin = 0.f;
        uint32_t sampleMask = 0;
        bool     alphaToCoverageEnable = false;
        bool     alphaToOneEnable = false;
    };
}