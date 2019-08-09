#pragma once

#include "Geometry/Algebra/MatrixAll.h"

namespace ZHU
{
    /* The circle is the intersection of the sphere |X-C|^2 = r^2 and the
     * plane Dot(N,X-C) = 0, where C is the circle center, r is the radius,
     * and N is a unit-length plane normal.
     */
    template<typename T>
    class Circle3
    {
    public:
        /* The default constructor sets center to (0,0,0), 
         *                              normal to (0,0,1), 
         *                          and radius to 1.
         */
        Circle3();
        Circle3(const Vector3<T>& center, const Vector3<T>& normal, T radius);

        inline bool operator==(const Circle3& other) const;
        inline bool operator!=(const Circle3& other) const { return !operator==(other); }
        inline bool operator< (const Circle3& other) const;
        inline bool operator<=(const Circle3& other) const { return operator<(other) || operator==(other); }
        inline bool operator> (const Circle3& other) const { return !operator<=(other); }
        inline bool operator>=(const Circle3& other) const { return !operator<(other); }

    public:
        Vector3<T> mCenter;
        Vector3<T> mNormal;
        T mRadius;
    };











    /*
                         TEMPLATE CLASS IMPLEMENTATION
      -------------------------------+----------------------------------
                               CLASS : Circle3
                           ARGUMENTS : <typename T>
                         GENERAL IMP : YES
              PARTIAL SPECIALIZATION : NO
                 FULL SPECIALIZATION : NO
      -------------------------------+----------------------------------
    */
    template<typename T>
    Circle3<T>::Circle3()
        : mCenter(Vector3<T>::Zero())
        , mNormal(Vector3<T>::UnitZ())
        , mRadius((T)1)
    {}


    template<typename T>
    Circle3<T>::Circle3(const Vector3<T>& center, 
                          const Vector3<T>& normal, 
                          T radius)
        : mCenter(center)
        , mNormal(normal)
        , mRadius(radius)
    {}


    template<typename T>
    inline bool Circle3<T>::operator==(const Circle3& other) const
    {
        return mCenter == other.mCenter
            && mNormal == other.mNormal
            && mRadius == other.mRadius;
    }

    template<typename T>
    inline bool Circle3<T>::operator< (const Circle3& other) const
    {
        if (mCenter < other.mCenter) return true;
        if (mCenter > other.mCenter) return false;

        if (mNormal < other.mNormal) return true;
        if (mNormal > other.mNormal) return false;

        return mRadius < other.mRadius;
    }
}