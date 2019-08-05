#pragma once

#include "Geometry/Algebra/Vector.h"

namespace ZHU
{
    /* Factorization into Euler angles is not necessarily unique.  Let the
     * integer indices for the axes be (N0,N1,N2), which must be in the set
     *   {(0,1,2),(0,2,1),(1,0,2),(1,2,0),(2,0,1),(2,1,0),
     *    (0,1,0),(0,2,0),(1,0,1),(1,2,1),(2,0,2),(2,1,2)}
     * Let the corresponding angles be (angleN0,angleN1,angleN2).  If the
     * result is ER_NOT_UNIQUE_SUM, then the multiple solutions occur because
     * angleN2+angleN0 is constant.  If the result is ER_NOT_UNIQUE_DIF, then
     * the multiple solutions occur because angleN2-angleN0 is constant.  In
     * either type of nonuniqueness, the function returns angleN0=0.
     */
    enum class EulerResult
    {
        INVALID,
        UNIQUE,
        NOT_UNIQUE_SUM,
        NOT_UNIQUE_DIF,
    };


    template<typename T>
    class EulerAngles
    {
    public:
        EulerAngles()
            : mResult(EulerResult::INVALID)
            , mAxis{ 0 }
            , mAngle{ (T)0 }
        {}
        EulerAngles(int i0, int i1, int i2, T a0, T a1, T a2)
            : mResult(EulerResult::UNIQUE)
            , mAxis{ i0, i1, i2 }
            , mAngle{ a0, a1, a2 }
        {}

    public:
        int mAxis[3];
        T mAngle[3];
        EulerResult mResult; // is set during conversions from rotation matrices, quaternions, or axis-angles.
    };
}