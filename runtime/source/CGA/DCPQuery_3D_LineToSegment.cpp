#include "GE2/CGA/internal/DCPQuery_3D_LineToSegment.h"

namespace CGA
{
    DCPQuery<Line, Segment>::ComputeResult 
        DCPQuery<Line, Segment>::operator()(const Line& line, const Segment& segment)
    {
        ComputeResult result;

        Point segCenter;
        Vector segDirection;
        double segExtent;
        segment.GetCenteredForm(&segCenter, &segDirection, &segExtent);

        Vector diff = line.GetOrigin() - segCenter;
        double a01 = -line.GetDirection().dot(segDirection);
        double b0 = diff.dot(line.GetDirection());
        double s0, s1;

        if (std::abs(a01) < 1.0)
        {
            // The line and segment are not parallel.
            double det = 1.0 - a01 * a01;
            double extDet = segExtent * det;
            double b1 = -diff.dot(segDirection);
            s1 = a01 * b0 - b1;

            if (s1 >= -extDet)
            {
                if (s1 <= extDet)
                {
                    // Two interior points are closest, one on the line and one
                    // on the segment.
                    s0 = (a01 * b1 - b0) / det;
                    s1 /= det;
                }
                else
                {
                    // The endpoint e1 of the segment and an interior point of
                    // the line are closest.
                    s1 = segExtent;
                    s0 = -(a01 * s1 + b0);
                }
            }
            else
            {
                // The endpoint e0 of the segment and an interior point of the
                // line are closest.
                s1 = -segExtent;
                s0 = -(a01 * s1 + b0);
            }
        }
        else
        {
            // The line and segment are parallel.  Choose the closest pair so that
            // one point is at segment origin.
            s1 = 0.0;
            s0 = -b0;
        }

        result.parameters[0] = s0;
        result.parameters[1] = s1;
        result.closestPoints[0] = line.GetOrigin() + s0 * line.GetDirection();
        result.closestPoints[1] = segCenter + s1 * segDirection;
        diff = result.closestPoints[0] - result.closestPoints[1];
        result.squaredDistance = diff.dot(diff);
        result.distance = std::sqrt(result.squaredDistance);
        return result;
    }
}