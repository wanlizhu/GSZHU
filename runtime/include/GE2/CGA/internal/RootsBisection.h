#pragma once

#include "DCPQuery_Common.h"

// Compute a root of a function F(t) on an interval [t0, t1].  The caller
// specifies the maximum number of iterations, in case you want limited
// accuracy for the root.  However, the function is designed for native types
// (Real = float/double).  If you specify a sufficiently large number of
// iterations, the root finder bisects until either F(t) is identically zero
// [a condition dependent on how you structure F(t) for evaluation] or the
// midpoint (t0 + t1)/2 rounds numerically to tmin or tmax.  Of course, it
// is required that t0 < t1.  The return value of Find is:
//   0: F(t0)*F(t1) > 0, we cannot determine a root
//   1: F(t0) = 0 or F(t1) = 0
//   2..maxIterations:  the number of bisections plus one
//   maxIterations+1:  the loop executed without a break (no convergence)

namespace GE2::CGA
{
    class GE2_IMPEXP RootsBisection final
    {
    public:
        // Use this function when F(t0) and F(t1) are not already known.
        static unsigned int Find(const std::function<double(double)>& F, 
                                 double t0, double t1,
                                 unsigned int maxIterations, double& root);

        // If f0 = F(t0) and f1 = F(t1) are already known, pass them to the
        // bisector.  This is useful when |f0| or |f1| is infinite, and you
        // can pass sign(f0) or sign(f1) rather than then infinity because
        // the bisector cares only about the signs of f.
        static unsigned int Find(const std::function<double(double)>& F, 
                                 double t0, double t1, 
                                 double f0, double f1,
                                 unsigned int maxIterations, double& root);
    };
}