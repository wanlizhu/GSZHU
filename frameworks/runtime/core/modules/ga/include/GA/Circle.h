#pragma once

#include "Common.h"

namespace GA2
{
    template<typename T, int N>
    class Circle
    {
    public:
        using Scalar = T;
        using VEC = Vector<T, N>;

        Circle() = default;
        Circle(const VEC& center, const VEC& normal, T radius)
            : mCenter(center)
            , mNormal(normal)
            , mRadius(radius)
        {}

        DEFINE_GETTERS(Center, VEC)
        DEFINE_GETTERS(Normal, VEC)
        DEFINE_GETTERS(Radius, T)
        DEFINE_TRIVIAL_COMPARISONS(Circle)

        operator bool() const { 
            return !mNormal.isApprox(VEC::Zero()) &&
                   !FLOAT_EQ(mRadius, 0.0); 
        }
        bool operator==(const Circle& other) const {
            return mCenter.isApprox(other.mCenter) &&
                   mNormal.isApprox(other.mNormal) &&
                   FLOAT_EQ(mRadius, other.mRadius);
        }
        bool operator<(const Circle& other) const { 
            return mCenter < other.mCenter; 
        }

    private:
        VEC mCenter = VEC::Zero();
        VEC mNormal = VEC::UnitY();
        T   mRadius = (T)0;
    };
}