#pragma once

#include "GE2/Utilities.h"

namespace GE2::CGA
{
    template<int N>
    class LineT
    {
    public:
        using PNT = PointT<N>;
        using VEC = VectorT<N>;

        LineT() = default;
        LineT(const PNT& origin, const VEC& direction)
            : mOrigin(origin)
            , mDirection(direction.stableNormalized())
        {}

        PNT&  GetOrigin()             { return mOrigin; }
        VEC&  GetDirection()          { return mDirection; }
        PNT   PointAt(double t) const { return mOrigin + (mDirection * t); }

        const PNT&  GetOrigin()    const { return mOrigin; }
        const VEC&  GetDirection() const { return mDirection; }

        operator bool()                    const { return !mDirection.isApprox(VEC::Zero()); }
        bool operator==(const LineT& line) const { return mOrigin.isApprox(line.mOrigin) && mDirection.isApprox(line.mDirection); }
        bool operator!=(const LineT& line) const { return !operator==(line); }

    private:
        PNT mOrigin;
        VEC mDirection;
    };

    using Line   = LineT<3>;
    using Line2D = LineT<2>;
}