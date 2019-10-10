#pragma once

#include "BasicTypes.h"
#include "BasicShapes.h"

namespace CGA
{
    struct LineToAlignedBox 
    {
        float distance;
        float squaredDistance;
        float lineOffset;
        Point closestPoint[2]; // [0] on the line, [1] on the box
    };

    void ComputeDistance3D(const Line& line, const AlignedBox& box, LineToAlignedBox* result);
}