#pragma once

#include "DCPQuery_Common.h"

// The 3D line-circle distance algorithm is described in
// "runtime/doc/Distance To Circle3.pdf"
// The notation used in the code matches that of the document.

namespace CGA
{
    template<>
    class CGA_API DCPQuery<Line, Circle>
    {
        struct ClosestInfo;
    public:
        // The possible number of closest line-circle pairs is 1, 2 or all circle
        // points.  If 1 or 2, numClosestPairs is set to this number and
        // 'equidistant' is false; the number of valid elements in lineClosest[]
        // and circleClosest[] is numClosestPairs.  If all circle points are
        // closest, the line must be C+t*N where C is the circle center, N is
        // the normal to the plane of the circle, and lineClosest[0] is set to C.
        // In this case, 'equidistant' is true and circleClosest[0] is set to
        // C+r*U, where r is the circle radius and U is a vector perpendicular
        // to N.
        struct ComputeResult
        {
            double distance = 0.0;
            double squaredDistance = 0.0;

            int   numClosestPoints = 0;
            Point lineClosest[2];
            Point circleClosest[2];
            bool  equidistant = false;
        };

        // Use nonpolynomial-based algorithm which uses bisection when "robust" is true,
        // otherwise, the polynomial-based algorithm is applied.
        ComputeResult operator()(const Line& line, const Circle& circle, bool robust = true);

    private:
        void ComputeDistance3D(const Line& line, const Circle& circle, ComputeResult* result);
        void ComputeDistance3D_NonPoly(const Line& line, const Circle& circle, ComputeResult* result);
    
        void GetPointPair(const Line& line, const Circle& circle,
                          const Point& D, double t,
                          Point& lineClosest, Point& circleClosest);
        // Bisect the function
        //   F(s) = s + m2b2 - r*m0sqr*s/sqrt(m0sqr*s*s + b1sqr)
        // on the specified interval [smin,smax].
        double Bisect(double m2b2, double rm0sqr, double m0sqr, double b1sqr,
                      double smin, double smax);
    };
}