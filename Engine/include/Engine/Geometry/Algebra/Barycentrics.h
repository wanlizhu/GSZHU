#pragma once

#include "Engine/Common/Math.h"
#include "Engine/Geometry/Algebra/Perpendicular.h"

namespace ZHU
{
    template<typename T, int N>
    class Barycentrics
    {
    public:
        Barycentrics(const Vector<T, N>& v0,
                     const Vector<T, N>& v1,
                     const Vector<T, N>& v2);
    
        bool operator()(const Vector<T, N>& point, T bary[N+1], T eps = (T)0) const;
    };
    
    
    
    
    
    
    
    
    
    
    
    /*
                        TEMPLATE CLASS IMPLEMENTATION
      -------------------------------+----------------------------------
                               CLASS : Barycentrics
                           ARGUMENTS : <typename T, int N>
                         GENERAL IMP : NO
              PARTIAL SPECIALIZATION : YES
                 FULL SPECIALIZATION : NO
      -------------------------------+----------------------------------
    */

    template<typename T>
    class Barycentrics<T, 2>
    {
    public:
        Barycentrics(const Vector<T, 2>& v0,
                     const Vector<T, 2>& v1,
                     const Vector<T, 2>& v2)
            : mV0(v0)
            , mV1(v1)
            , mV2(v2)
        {}

        bool operator()(const Vector<T, 2>& point, T bary[3], T eps = (T)0) const
        {
            // Compute the vectors relative to V2 of the triangle.
            Vector2<T> diff[3] = { mV0 - mV2, mV1 - mV2, point - mV2 };

            T det = diff[0].dot(Perpendicular()(diff[1]));
            if (det < -eps || det > eps) {
                T invDet = ((T)1) / det;
                bary[0] = diff[2].dot(Perpendicular()(diff[1])) * invDet;
                bary[1] = diff[0].dot(Perpendicular()(diff[2])) * invDet;
                bary[2] = (T)1 - bary[0] - bary[1];
                return true;
            }

            for (int i = 0; i < 3; ++i) {
                bary[i] = (T)0;
            }
            return false;
        }

    private:
        const Vector<T, 2> mV0;
        const Vector<T, 2> mV1;
        const Vector<T, 2> mV2;
    };


    template<typename T>
    class Barycentrics<T, 3>
    {
    public:
        Barycentrics(const Vector<T, 3>& v0,
                     const Vector<T, 3>& v1,
                     const Vector<T, 3>& v2,
                     const Vector<T, 3>& v3)
            : mV0(v0)
            , mV1(v1)
            , mV2(v2)
            , mV3(v3)
        {}

        bool operator()(const Vector<T, 3> & point, T bary[4], T eps = (T)0) const
        {
            // Compute the vectors relative to V3 of the tetrahedron.
            Vector3<T> diff[4] = { mV0 - mV3, mV1 - mV3, mV2 - mV3, point - mV3 };

            T det = diff[0].dot(diff[1].cross(diff[2]));
            if (det < -eps || det > eps) {
                T invDet = ((T)1) / det;
                bary[0] = diff[3].dot(diff[1].cross(diff[2])) * invDet;
                bary[1] = diff[3].dot(diff[2].cross(diff[0])) * invDet;
                bary[2] = diff[3].dot(diff[0].cross(diff[1])) * invDet;
                bary[3] = (T)1 - bary[0] - bary[1] - bary[2];
                return true;
            }

            for (int i = 0; i < 4; ++i) {
                bary[i] = (T)0;
            }
            return false;
        }

    private:
        const Vector<T, 2> mV0;
        const Vector<T, 2> mV1;
        const Vector<T, 2> mV2;
        const Vector<T, 2> mV3;
    };
}