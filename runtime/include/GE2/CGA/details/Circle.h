#pragma once

#include "BasicTypes.h"

// The circle is the intersection of the sphere |X-C|^2 = r^2 and the
// plane Dot(N,X-C) = 0, where C is the circle center, r is the radius,
// and N is a unit-length plane normal.

namespace CGA
{
    class Circle
    {
    public:
        Circle()
            : mCenter(0.0, 0.0, 0.0)
            , mNormal(0.0, 0.0, 1.0)
            , mRadius(1.0)
        {}
        Circle(const Point& center, const Vector& normal, double radius)
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

        operator bool() const { return !mNormal.isApprox(Vector::Zero()) && !(FLOAT_EQ(mRadius, 0.0)); }
        bool operator==(const Circle& circle) const {
            return mCenter.isApprox(circle.mCenter) && mNormal.isApprox(circle.mNormal) && FLOAT_EQ(mRadius, circle.mRadius);
        }
        bool operator!=(const Circle& circle) const { return !operator==(circle); }

    private:
        Point  mCenter;
        Vector mNormal;
        double mRadius;
    };
}