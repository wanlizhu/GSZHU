#pragma once

#include "Common.h"

namespace GA2
{
    template<typename T, int N>
    class Line
    {
    public:
        using Scalar = T;
        using VEC = Vector<T, N>;

        Line() = default;
        Line(const VEC& origin, const VEC& direction)
            : mOrigin(origin)
            , mDirection(direction)
        {}

        DEFINE_GETTERS(Origin,    VEC)
        DEFINE_GETTERS(Direction, VEC)
        DEFINE_TRIVIAL_COMPARISONS(Line)

        operator bool() const { return !mDirection.isApprox(VEC::Zero()); }
        bool operator<(const Line& line) const { return mOrigin < line.mOrigin; }
        bool operator==(const Line& line) const { 
            return mOrigin.isApprox(line.mOrigin) && 
                   mDirection.isApprox(line.mDirection);
        }

        VEC PointAt(double t) const { return mOrigin + (t * mDirection); }

    private:
        VEC mOrigin;
        VEC mDirection;
    };
}