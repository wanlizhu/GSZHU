#pragma once

#include "DCPQuery_Common.h"

// The 3D circle-circle distance algorithm is described in "runtime/doc/Distance to Circles in 3D.pdf"

namespace CGA
{
    template<>
    class CGA_API DCPQuery<Circle, Circle>
    {
        class SCPolynomial;
        struct ClosestInfo;
    public:
        struct ComputeResult
        {
            double distance = 0.0;
            double squaredDistance = 0.0;
            
            int  numClosestPoints = 0;
            Point circle0Closest[2];
            Point circle1Closest[2];
            bool equidistant = false;
        };

        ComputeResult operator()(const Circle& circle0, const Circle& circle1);

    protected:
        // The two circles are in parallel planes where D = C1 - C0, 
        // the difference of circle centers.
        void ComputeParallelPlanes(const Circle& circle0,
                                   const Circle& circle1, 
                                   const Vector& D,
                                   ComputeResult* result);
    };
}