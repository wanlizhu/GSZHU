#pragma once

#include "DCPQuery_Common.h"

namespace GE2::CGA
{
    template<>
    class GE2_IMPEXP DCPQuery<Point, AlignedBox>
    {
    public:
        struct ComputeResult
        {
            double distance = 0.0;
            double squaredDistance = 0.0;
            Point  boxClosest;
        };

        ComputeResult operator()(const Point& point, const AlignedBox& box);

    protected:
        // On input, 'point' is the difference of the query point and the box center. 
        // On output, 'point' is the point on the box closest to the query point.
        void ComputeDistance3D(Point& point, 
                               const Vector& boxExtent,
                               ComputeResult* result);
    };
}