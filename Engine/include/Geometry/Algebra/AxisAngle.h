#pragma once

#include "Geometry/Algebra/Vector.h"

namespace ZHU
{
    template<typename T, int N>
    class AxisAngle 
    {
    public:
        AxisAngle() {}
        AxisAngle(const Vector<T, N>& axis, T angle) 
            : mAxis(axis), mAngle(angle) 
        {}

    public:
        Vector<T, N> mAxis;
        T mAngle;
    };
}