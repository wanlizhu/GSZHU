#pragma once

#include "Engine/Common/Math.h"

namespace ZHU
{
    /* Axis-angle representation for N = 3 or N = 4.  When N = 4, the axis
     * must be a vector of the form (x,y,z,0) [affine representation of the
     * 3-tuple direction].
    */
    template<typename T, int N>
    class AxisAngle
    {
    public:
        AxisAngle()
        {
            static_assert(N == 3 || N == 4, "Dimension must be 3 or 4.");
        }

        AxisAngle(const Vector<T, N>& axis_, T angle_)
            : axis(axis_)
            , angle(angle_)
        {
            static_assert(N == 3 || N == 4, "Dimension must be 3 or 4.");
        }

    public:
        Vector<T, N> axis;
        T angle;
    };
}