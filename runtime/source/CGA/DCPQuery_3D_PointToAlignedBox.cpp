#include "GE2/CGA/details/DCPQuery_3D_PointToAlignedBox.h"

namespace CGA
{
    DCPQuery<Point, AlignedBox>::ComputeResult 
        DCPQuery<Point, AlignedBox>::operator()(const Point& point, const AlignedBox& box)
    {
        // Translate the point and box so that the box has center at the origin
        Point  boxCenter = (box.max() + box.min()) * 0.5;
        Vector boxExtent = (box.max() - box.min()) * 0.5;
        Vector closest = point - boxCenter;

        ComputeResult result;
        ComputeDistance3D(closest, boxExtent, &result);
        
        // Compute the closest point on the box
        result.boxClosest = boxCenter + closest;
        return result;
    }

    void DCPQuery<Point, AlignedBox>::ComputeDistance3D(Point& point,
                                                        const Vector& boxExtent,
                                                        DCPQuery<Point, AlignedBox>::ComputeResult* result)
    {
        static const int N = 3;
        result->squaredDistance = 0.0;
        for (int i = 0; i < N; ++i)
        {
            if (point[i] < -boxExtent[i])
            {
                double delta = point[i] + boxExtent[i];
                result->squaredDistance += delta * delta;
                point[i] = -boxExtent[i];
            }
            else if (point[i] > boxExtent[i])
            {
                double delta = point[i] - boxExtent[i];
                result->squaredDistance += delta * delta;
                point[i] = boxExtent[i];
            }
        }
        result->distance = std::sqrt(result->squaredDistance);
    }
}