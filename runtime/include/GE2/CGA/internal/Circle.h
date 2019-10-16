#pragma once

#include "GE2/Utilities.h"

// The circle is the intersection of the sphere |X-C|^2 = r^2 and the
// plane Dot(N,X-C) = 0, where C is the circle center, r is the radius,
// and N is a unit-length plane normal.

namespace GE2::CGA
{
    template<int N>
    class CircleT
    {
    public:
        using PNT = PointT<N>;
        using VEC = VectorT<N>;

        CircleT()
            : mCenter(PNT::Zero())
            , mNormal(VEC::UnitY())
        {}
        CircleT(const PNT& center, const VEC& normal, double radius)
            : mCenter(center)
            , mNormal(normal)
            , mRadius(radius)
        {}

        PNT&    GetCenter() { return mCenter; }
        VEC&    GetNormal() { return mNormal; }
        double& GetRadius() { return mRadius; }

        const PNT&    GetCenter() const { return mCenter; }
        const VEC&    GetNormal() const { return mNormal; }
        const double& GetRadius() const { return mRadius; }

        operator bool() const {
            return !mNormal.isApprox(VEC::Zero()) &&
                   !(FLOAT_EQ(mRadius, 0.0));
        }
        bool operator==(const CircleT& circle) const {
            return mCenter.isApprox(circle.mCenter) &&
                   mNormal.isApprox(circle.mNormal) &&
                  FLOAT_EQ(mRadius, circle.mRadius);
        }
        bool operator!=(const CircleT& circle) const { return !operator==(circle); }

    private:
        PNT    mCenter;
        VEC    mNormal;
        double mRadius = 0.0;
    };

    using Circle   = CircleT<3>;
    using Circle2D = CircleT<2>;
}