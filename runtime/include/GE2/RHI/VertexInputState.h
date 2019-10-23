#pragma once

#include "GE2/BasicTypes.h"
#include "GE2/Utilities.h"
#include "EResourceFormat.h"

namespace GE2::RHI
{
    enum class EVertexInputRate
    {
        PerVertex,
        PerInstance,
    };

    struct VertexInputBinding
    {
        uint32_t         binding = 0;
        uint32_t         stride = 0;
        EVertexInputRate inputRate = EVertexInputRate::PerVertex;
    };

    struct VertexInputAttribute
    {
        uint32_t        location = 0;
        uint32_t        binding = 0;
        EResourceFormat format = EResourceFormat::RGB32Float;
        uint32_t        offset = 0;
    };

    struct GE2_IMPEXP VertexInputState
    {
        LIST<VertexInputBinding>   vertexBindings;
        LIST<VertexInputAttribute> vertexAttributes;
    };
}