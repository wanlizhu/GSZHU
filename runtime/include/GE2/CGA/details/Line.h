#pragma once

#include "BasicTypes.h"

namespace CGA
{
    template<int DIM>
    class LineT
    {
    public:
        LineT() = default;
        LineT(const Point& origin, const Vector& direction)
            : mOrigin(origin)
            , mDirection(direction.stableNormalized())
        {}

        Point& GetOrigin() { return mOrigin; }
        Vector& GetDirection() { return mDirection; }
        Point   PointAt(double t) const { return mOrigin + (mDirection * t); }

        const Point& GetOrigin()    const { return mOrigin; }
        const Vector& GetDirection() const { return mDirection; }

        operator bool()                    const { return !mDirection.isApprox(Vector::Zero()); }
        bool operator==(const LineT& line) const { return mOrigin.isApprox(line.mOrigin) && mDirection.isApprox(line.mDirection); }
        bool operator!=(const LineT& line) const { return !operator==(line); }

    private:
        Point  mOrigin;
        Vector mDirection;
    };

    using Line   = LineT<3>;
    using Line2D = LineT<2>;
}