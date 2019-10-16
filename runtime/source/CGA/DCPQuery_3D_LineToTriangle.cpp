#include "GE2/CGA/internal/DCPQuery_3D_LineToTriangle.h"

namespace GE2::CGA
{
    DCPQuery<Line, Triangle>::ComputeResult 
        DCPQuery<Line, Triangle>::operator()(const Line& line, const Triangle& triangle)
    {
        ComputeResult result;

        // Test if line intersects triangle.  If so, the squared distance is zero.
        Vector edge0 = triangle[1] - triangle[0];
        Vector edge1 = triangle[2] - triangle[0];
        Vector normal = UnitCross(edge0, edge1);
        double NdD = normal.dot(line.GetDirection());
        if (std::abs(NdD) > 0.0)
        {
            // The line and triangle are not parallel, so the line intersects
            // the plane of the triangle.
            Vector diff = line.GetOrigin() - triangle[0];
            Vector basis[3];  // {D, U, V}
            basis[0] = line.GetDirection();
            ComputeOrthogonalComplement(1, basis);
            double UdE0 = basis[1].dot(edge0);
            double UdE1 = basis[1].dot(edge1);
            double UdDiff = basis[1].dot(diff);
            double VdE0 = basis[2].dot(edge0);
            double VdE1 = basis[2].dot(edge1);
            double VdDiff = basis[2].dot(diff);
            double invDet = (1.0) / (UdE0 * VdE1 - UdE1 * VdE0);

            // Barycentric coordinates for the point of intersection.
            double b1 = (VdE1 * UdDiff - UdE1 * VdDiff) * invDet;
            double b2 = (UdE0 * VdDiff - VdE0 * UdDiff) * invDet;
            double b0 = 1.0 - b1 - b2;

            if (b0 >= 0.0 && b1 >= 0.0 && b2 >= 0.0)
            {
                // Line parameter for the point of intersection.
                double DdE0 = line.GetDirection().dot(edge0);
                double DdE1 = line.GetDirection().dot(edge1);
                double DdDiff = line.GetDirection().dot(diff);
                result.lineParameter = b1 * DdE0 + b2 * DdE1 - DdDiff;

                // Barycentric coordinates for the point of intersection.
                result.triangleParameters[0] = b0;
                result.triangleParameters[1] = b1;
                result.triangleParameters[2] = b2;

                // The intersection point is inside or on the triangle.
                result.closestPoints[0] = line.GetOrigin() + result.lineParameter * line.GetDirection();
                result.closestPoints[1] = triangle[0] + b1 * edge0 + b2 * edge1;

                result.distance = 0.0;
                result.squaredDistance = 0.0;
                return result;
            }
        }

        // Either (1) the line is not parallel to the triangle and the point of
        // intersection of the line and the plane of the triangle is outside the
        // triangle or (2) the line and triangle are parallel.  Regardless, the
        // closest point on the triangle is on an edge of the triangle.  Compare
        // the line to all three edges of the triangle.
        result.distance = std::numeric_limits<double>::max();
        result.squaredDistance = std::numeric_limits<double>::max();
        for (int i0 = 2, i1 = 0; i1 < 3; i0 = i1++)
        {
            Vector segCenter = (0.5) * (triangle[i0] + triangle[i1]);
            Vector segDirection = triangle[i1] - triangle[i0];
            double segExtent = (0.5) * Normalize(segDirection);
            Segment segment(segCenter, segDirection, segExtent);

            DCPQuery<Line, Segment> query;
            auto lsResult = query(line, segment);
            if (lsResult.squaredDistance < result.squaredDistance)
            {
                result.squaredDistance = lsResult.squaredDistance;
                result.distance = lsResult.distance;
                result.lineParameter = lsResult.parameters[0];
                result.triangleParameters[i0] = (0.5) * (1.0 - lsResult.parameters[0] / segExtent);
                result.triangleParameters[i1] = 1.0 - result.triangleParameters[i0];
                result.triangleParameters[3 - i0 - i1] = 0.0;
                result.closestPoints[0] = lsResult.closestPoints[0];
                result.closestPoints[1] = lsResult.closestPoints[1];
            }
        }

        return result;
    }
}