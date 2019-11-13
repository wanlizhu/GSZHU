#pragma once

#include "Line.h"
#include "Circle.h"
#include "DCPQuery.h"

namespace GA
{
    // The possible number of closest line-circle pairs is 1, 2 or all circle
    // points.  If 1 or 2, numClosestPairs is set to this number and
    // 'equidistant' is false; the number of valid elements in lineClosest[]
    // and circleClosest[] is numClosestPairs.  If all circle points are
    // closest, the line must be C+t*N where C is the circle center, N is
    // the normal to the plane of the circle, and lineClosest[0] is set to C.
    // In this case, 'equidistant' is true and circleClosest[0] is set to
    // C+r*U, where r is the circle radius and U is a vector perpendicular
    // to N.
    template<typename T, int N>
    class DCPQuery<Line<T, N>, Circle<T, N>>
    {
    public:
        using Scalar = T;
        using VEC    = Vector<T, N>;
        using ShapeA = Line<T, N>;
        using ShapeB = Circle<T, N>;

        struct Result 
        {
            Scalar distance        = Scalar(0);
            Scalar squaredDistance = Scalar(0);

            int  numClosestPairs = 0;
            VEC  lineClosest[2];
            VEC  circleClosest[2];
            bool equidistant = false;
        };

        Result operator(const ShapeA& line, const ShapeB& circle, bool robust = true);

    private:
        
    };
}