#pragma once

#include "Common.h"

namespace GA
{
    template<typename T, int N>
    class Segment
    {
    public:
        using Scalar = T;
        using VEC = Vector<T, N>;

        Segment()
            : mPoints{ VEC::UnitX(), -VEC::UNitX() }
        {}
        Segment(const VEC& p0, const VEC& p1)
            : mPoints{ p0, p1 }
        {}
        Segment(const std::array<VEC, 2>& list)
            : mPoints(list)
        {}
        Segment(const VEC& center, const VEC& direction, T extent) {
            SetCenteredForm(center, direction, extent);
        }

        DEFINE_GETTERS(Points, std::array<VEC, 2>)
        DEFINE_TRIVIAL_COMPARISONS(Segment)

        VEC&       operator[](int i)       { return mPoints[i]; }
        const VEC& operator[](int i) const { return mPoints[i]; }
        operator bool() const { return !mPoints[0].isApprox(mPoints[1]); }
        bool operator==(const Segment& other) const { 
            return mPoints[0].isApprox(other.mPoints[0]) && 
                   mPoints[1].isApprox(other.mPoints[1]);
        }
        bool operator<(const Segment& other) const { 
            return mPoints[0] < other.mPoints[0];
        }

        void SetCenteredForm(const VEC& center, const VEC& direction, T extent) {
            mPoints[0] = center - (extent * direction);
            mPoints[1] = center + (extent * direction);
        }

        void GetCenteredForm(VEC* center, VEC* direction, T* extent) {
            *center = T(0.5) * (mPoints[0] + mPoints[1]);
            *direction = mPoints[1] - mPoints[0];
            *extent = T(0.5) * Normalize(*direction);
        }

    private:
        std::array<VEC, 2> mPoints;
    };
}