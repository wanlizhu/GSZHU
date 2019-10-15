#pragma once

#include "DCPQuery_Common.h"
#include "DCPQuery_3D_LineToSegment.h"

namespace CGA
{
    template<>
    class CGA_API DCPQuery<Line, Triangle>
    {
    public:
        struct ComputeResult
        {
            double distance = 0.0;
            double squaredDistance = 0.0;
            double lineParameter = 0.0;
            double triangleParameters[3] = { 0.0 };
            Point  closestPoints[2];
        };

        ComputeResult operator()(const Line& line, const Triangle& triangle);
    };
}