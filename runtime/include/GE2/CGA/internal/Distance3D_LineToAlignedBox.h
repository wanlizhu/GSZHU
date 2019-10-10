#pragma once

#include "BasicTypes.h"
#include "BasicShapes.h"
#include "../DCPQuery.h"

namespace CGA
{
    struct LineToAlignedBox 
    {
        double distance;
        double squaredDistance;
        double lineParameter;
        Point closestPoint[2]; // [0] on the line, [1] on the box
    };

    CGA_API void ComputeDistance3D(const Line& line, const AlignedBox& box, LineToAlignedBox* result);
    
    template<>
    class CGA_API DCPQuery<Line, AlignedBox>
    {
    public:
        using ComputeResult = LineToAlignedBox;

        ComputeResult operator()(const Line& line, const AlignedBox& box)
        {
            ComputeResult result;
            ComputeDistance3D(line, box, &result);
            return result;
        }
    };
}