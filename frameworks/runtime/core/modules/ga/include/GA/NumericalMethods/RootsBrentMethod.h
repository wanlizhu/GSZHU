#pragma once

#include <vector>
#include <map>
#include <algorithm>
#include <functional>
#include <cmath>
#include <type_traits>
#include <optional>

namespace GA
{
    // This is an implementation of Brent's Method for computing a root of a
    // function on an interval [t0,t1] for which F(t0)*F(t1) < 0.  The method
    // uses inverse quadratic interpolation to generate a root estimate but
    // falls back to inverse linear interpolation (secant method) if
    // necessary.  Moreover, based on previous iterates, the method will fall
    // back to bisection when it appears the interpolated estimate is not of
    // sufficient quality.
    //
    //   maxIterations:
    //       The maximum number of iterations used to locate a root.  This
    //       should be positive.
    //   negFTolerance, posFTolerance:
    //       The root estimate t is accepted when the function value F(t)
    //       satisfies negFTolerance <= F(t) <= posFTolerance.  The values
    //       must satisfy:  negFTolerance <= 0, posFTolerance >= 0.
    //   stepTTolerance:
    //       Brent's Method requires additional tests before an interpolated
    //       t-value is accepted as the next root estimate.  One of these
    //       tests compares the difference of consecutive iterates and
    //       requires it to be larger than a user-specified t-tolerance (to
    //       ensure progress is made).  This parameter is that tolerance
    //       and should be nonnegative.
    //   convTTolerance:
    //       The root search is allowed to terminate when the current
    //       subinterval [tsub0,tsub1] is sufficiently small, say,
    //       |tsub1 - tsub0| <= tolerance.  This parameter is that tolerance
    //       and should be nonnegative.

    template<typename T>
    class RootsBrentsMethod
    {
    public:
        using Scalar = T;
        struct Tolerance
        {
            std::optional<Scalar> negFTolerance;
            std::optional<Scalar> posFTolerance;
            std::optional<Scalar> stepTTolerance;
            std::optional<Scalar> convTTolerance;
        };

        // if F(t0)*F(t1) > 0 or t0 >= t1, 
        // the function will return invalid value
        static bool FindRootInside(const std::function<Scalar(Scalar)>& F, 
                                   Scalar t0, 
                                   Scalar t1,
                                   int maxIterations,
                                   Tolerance tolerance,
                                   Scalar* root);
    };
}