#pragma once

#include "DCPQuery_Common.h"

namespace GE2::CGA
{
    template<>
    class GE2_IMPEXP DCPQuery<Line, Segment>
    {
    public:
        struct ComputeResult
        {
            double distance = 0.0;
            double squaredDistance = 0.0;
            double parameters[2];
            Point  closestPoints[2];
        };

        // The centered form of the 'segment' is used.  Thus, parameter[1] of
        // the result is in [-e,e], where e = |segment.p[1] - segment.p[0]|/2.
        ComputeResult operator()(const Line& line, const Segment& segment);
    };
}