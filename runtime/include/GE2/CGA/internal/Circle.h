#pragma once

#include "BasicTypes.h"

// The circle is the intersection of the sphere |X-C|^2 = r^2 and the
// plane Dot(N,X-C) = 0, where C is the circle center, r is the radius,
// and N is a unit-length plane normal.

namespace CGA
{
    template<typename T>
    class CircleT
    {
    public:
        CircleT()
            : mCenter((T)0, (T)0, (T)0)
            , mNormal((T)0, (T)0, (T)1)
            , mRadius((T)1)
        {}
        CircleT(const Point& center, const Vector& normal, T radius)
            : mCenter(center)
            , mNormal(normal)
            , mRadius(radius)
        {}

        Point&  GetCenter() { return mCenter; }
        Vector& GetNormal() { return mNormal; }
        double& GetRadius() { return mRadius; }

        const Point&  GetCenter() const { return mCenter; }
        const Vector& GetNormal() const { return mNormal; }
        const double& GetRadius() const { return mRadius; }

        operator bool() const { return !mNormal.isApprox(Vector::Zero()) && !(FLOAT_EQ(mRadius, (T)0)); }
        bool operator==(const CircleT& circle) const { return mCenter.isApprox(circle.mCenter) &&
                                                              mNormal.isApprox(circle.mNormal) &&
                                                              FLOAT_EQ(mRadius, circle.mRadius); }
        bool operator!=(const CircleT& circle) const { return !operator==(circle); }

    private:
        Point  mCenter;
        Vector mNormal;
        T      mRadius;
    };

    using Circle = CircleT<double>;
}