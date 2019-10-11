#pragma once

#include "DCPQuery_Common.h"

namespace CGA
{
    template<>
    class CGA_API DCPQuery<Line, AlignedBox>
    {
    public:
        struct ComputeResult
        {
            double distance = 0.0;
            double squaredDistance = 0.0;
            double lineParameter = 0.0;
            Point  closestPoint[2]; // [0] on the line, [1] on the box
        };

        ComputeResult operator()(const Line& line, const AlignedBox& box);

    protected:
        void ComputeDistance3D(const Line& line, 
                               const AlignedBox& box, 
                               ComputeResult* result);
    };
}