#pragma once

#include "BasicTypes.h"

namespace CGA
{
    template<typename T, int DIM>
    class LineT
    {
    public:
        using VECTOR = Vector<T, DIM>;
        using POINT  = Vector<T, DIM>;

        LineT() = default;
        LineT(const POINT& origin, const VECTOR& direction)
            : mOrigin(origin)
            , mDirection(direction.stableNormalized())
        {}

        POINT&  GetOrigin()        { return mOrigin; }
        VECTOR& GetDirection()     { return mDirection; }
        POINT   PointAt(T t) const { return mOrigin + (mDirection * t); }

        const POINT&  GetOrigin()    const { return mOrigin; }
        const VECTOR& GetDirection() const { return mDirection; }

        operator bool()                    const { return !mDirection.isApprox(VECTOR::Zero()); }
        bool operator==(const LineT& line) const { return mOrigin == line.mOrigin && mDirection == line.mDirection; }
        bool operator!=(const LineT& line) const { return !operator==(line); }

    private:
        POINT  mOrigin;
        VECTOR mDirection;
    };

    using Line   = LineT<float, 3>;
    using Line2D = LineT<float, 2>;
}