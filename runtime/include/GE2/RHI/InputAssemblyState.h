#pragma once

#include "GE2/BasicTypes.h"
#include "GE2/Utilities.h"
#include "ForwardDeclarations.h"

namespace GE2::RHI
{
    enum class EPrimitiveTopology
    {
        PointList,
        LineList,
        LineStrip,
        TriangleList,
        TriangleStrip,
        TriangleFan,
        LineListWithAdjacency,
        LineStripWithAdjacency,
        TriangleListWithAdjacency,
        TriangleStripWithAdjacency,
        PatchList,
    };

    struct GE2_IMPEXP InputAssemblyState
    {
        EPrimitiveTopology topology;
        bool               primitiveRestartEnable = false;
    };
}