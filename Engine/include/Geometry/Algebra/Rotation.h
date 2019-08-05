#pragma once

#include "Geometry/Algebra/MatrixAll.h"
#include "Geometry/Algebra/AxisAngle.h"
#include "Geometry/Algebra/EulerAngles.h"
#define _USE_MATH_DEFINES 
#include <math.h>

namespace ZHU
{
    /*
     * Conversions among various representations of rotations.  The value of
     * N must be 3 or 4.  The latter case supports affine algebra when you use
     * 4-tuple vectors (w-component is 1 for points and 0 for vector) and 4x4
     * matrices for affine transformations.  Rotation axes must be unit length.
     * The angles are in radians.  The Euler angles are in world coordinates;
     * we have not yet added support for body coordinates.
     *
     * (1) Convert a rotation matrix to a quaternion.
     *
     * x^2 = (+r00 - r11 - r22 + 1)/4
     * y^2 = (-r00 + r11 - r22 + 1)/4
     * z^2 = (-r00 - r11 + r22 + 1)/4
     * w^2 = (+r00 + r11 + r22 + 1)/4
     * x^2 + y^2 = (1 - r22)/2
     * z^2 + w^2 = (1 + r22)/2
     * y^2 - x^2 = (r11 - r00)/2
     * w^2 - z^2 = (r11 + r00)/2
     * x*y = (r01 + r10)/4
     * x*z = (r02 + r20)/4
     * y*z = (r12 + r21)/4
     * [GTE_USE_MAT_VEC]
     *   x*w = (r21 - r12)/4
     *   y*w = (r02 - r20)/4
     *   z*w = (r10 - r01)/4
     * [GTE_USE_VEC_MAT]
     *   x*w = (r12 - r21)/4
     *   y*w = (r20 - r02)/4
     *   z*w = (r01 - r10)/4
     *
     * If Q is the 4x1 column vector (x,y,z,w), the previous equations give us
     *         +-                  -+
     *         | x*x  x*y  x*z  x*w |
     * Q*Q^T = | y*x  y*y  y*z  y*w |
     *         | z*x  z*y  z*z  z*w |
     *         | w*x  w*y  w*z  w*w |
     *         +-                  -+
     * The code extracts the row of maximum length, normalizing it to obtain
     * the result q.
     *
     * (2) Convert a quaterion q = x*i + y*j + z*k + w to a rotation matrix.
     * [GTE_USE_MAT_VEC]
     *     +-           -+   +-                                     -+
     * R = | r00 r01 r02 | = | 1-2y^2-2z^2  2(xy-zw)     2(xz+yw)    |
     *     | r10 r11 r12 |   | 2(xy+zw)     1-2x^2-2z^2  2(yz-xw)    |
     *     | r20 r21 r22 |   | 2(xz-yw)     2(yz+xw)     1-2x^2-2y^2 |
     *     +-           -+   +-                                     -+
     * [GTE_USE_VEC_MAT]
     *     +-           -+   +-                                     -+
     * R = | r00 r01 r02 | = | 1-2y^2-2z^2  2(xy+zw)     2(xz-yw)    |
     *     | r10 r11 r12 |   | 2(xy-zw)     1-2x^2-2z^2  2(yz+xw)    |
     *     | r20 r21 r22 |   | 2(xz+yw)     2(yz-xw)     1-2x^2-2y^2 |
     *     +-           -+   +-                                     -+
     *
     * (3) Convert a rotation matrix to an axis-angle pair.  Let (x0,x1,x2) be the
     * axis let t be an angle of rotation.  The rotation matrix is
     * [GTE_USE_MAT_VEC]
     *   R = I + sin(t)*S + (1-cos(t))*S^2
     * or
     * [GTE_USE_VEC_MAT]
     *   R = I - sin(t)*S + (1-cos(t))*S^2
     * where I is the identity and S = {{0,-x2,x1},{x2,0,-x0},{-x1,x0,0}}
     * where the inner-brace triples are the rows of the matrix.  If t > 0,
     * R represents a counterclockwise rotation; see the comments for the
     * constructor Matrix3x3(axis,angle).  It may be shown that cos(t) =
     * (trace(R)-1)/2 and R - Transpose(R) = 2*sin(t)*S.  As long as sin(t) is
     * not zero, we may solve for S in the second equation, which produces the
     * axis direction U = (S21,S02,S10).  When t = 0, the rotation is the
     * identity, in which case any axis direction is valid; we choose (1,0,0).
     * When t = pi, it must be that R - Transpose(R) = 0, which prevents us
     * from extracting the axis.  Instead, note that (R+I)/2 = I+S^2 = U*U^T,
     * where U is a unit-length axis direction.
     *
     * (4) Convert an axis-angle pair to a rotation matrix.  Assuming (x0,x1,x2)
     * is for a right-handed world (x0 to right, x1 up, x2 out of plane of
     * page), a positive angle corresponds to a counterclockwise rotation from
     * the perspective of an observer looking at the origin of the plane of
     * rotation and having view direction the negative of the axis direction.
     * The coordinate-axis rotations are the following, where
     * unit(0) = (1,0,0), unit(1) = (0,1,0), unit(2) = (0,0,1),
     * [GTE_USE_MAT_VEC]
     *   R(unit(0),t) = {{ 1, 0, 0}, { 0, c,-s}, { 0, s, c}}
     *   R(unit(1),t) = {{ c, 0, s}, { 0, 1, 0}, {-s, 0, c}}
     *   R(unit(2),t) = {{ c,-s, 0}, { s, c, 0}, { 0, 0, 1}}
     * or
     * [GTE_USE_VEC_MAT]
     *   R(unit(0),t) = {{ 1, 0, 0}, { 0, c, s}, { 0,-s, c}}
     *   R(unit(1),t) = {{ c, 0,-s}, { 0, 1, 0}, { s, 0, c}}
     *   R(unit(2),t) = {{ c, s, 0}, {-s, c, 0}, { 0, 0, 1}}
     * where c = cos(t), s = sin(t), and the inner-brace triples are rows of
     * the matrix.  The general matrix is
     * [GTE_USE_MAT_VEC]
     *       +-                                                          -+
     *   R = | (1-c)*x0^2  + c     (1-c)*x0*x1 - s*x2  (1-c)*x0*x2 + s*x1 |
     *       | (1-c)*x0*x1 + s*x2  (1-c)*x1^2  + c     (1-c)*x1*x2 - s*x0 |
     *       | (1-c)*x0*x2 - s*x1  (1-c)*x1*x2 + s*x0  (1-c)*x2^2  + c    |
     *       +-                                                          -+
     * [GTE_USE_VEC_MAT]
     *       +-                                                          -+
     *   R = | (1-c)*x0^2  + c     (1-c)*x0*x1 + s*x2  (1-c)*x0*x2 - s*x1 |
     *       | (1-c)*x0*x1 - s*x2  (1-c)*x1^2  + c     (1-c)*x1*x2 + s*x0 |
     *       | (1-c)*x0*x2 + s*x1  (1-c)*x1*x2 - s*x0  (1-c)*x2^2  + c    |
     *       +-                                                          -+
     *
     * (5) Convert a rotation matrix to Euler angles.  Factorization into Euler
     * angles is not necessarily unique.  If the result is ER_NOT_UNIQUE_SUM,
     * then the multiple solutions occur because angleN2+angleN0 is constant.
     * If the result is ER_NOT_UNIQUE_DIF, then the multiple solutions occur
     * because angleN2-angleN0 is constant.  In either type of nonuniqueness,
     * the function returns angleN0=0.
     *
     * (6) Convert Euler angles to a rotation matrix.  The three integer inputs
     * are in {0,1,2} and correspond to world directions unit(0) = (1,0,0),
     * unit(1) = (0,1,0), or unit(2) = (0,0,1).  The triples (N0,N1,N2) must
     * be in the following set,
     *   {(0,1,2),(0,2,1),(1,0,2),(1,2,0),(2,0,1),(2,1,0),
     *    (0,1,0),(0,2,0),(1,0,1),(1,2,1),(2,0,2),(2,1,2)}
     * The rotation matrix is
     *   [GTE_USE_MAT_VEC]
     *     R(unit(N2),angleN2)*R(unit(N1),angleN1)*R(unit(N0),angleN0)
     * or
     *   [GTE_USE_VEC_MAT]
     *     R(unit(N0),angleN0)*R(unit(N1),angleN1)*R(unit(N2),angleN2)
     * The conventions of constructor Matrix3(axis,angle) apply here as well.
     *
     * NOTE:  The reversal of order is chosen so that a rotation matrix built
     * with one multiplication convention is the transpose of the rotation
     * matrix built with the other multiplication convention.  Thus,
     *   [GTE_USE_MAT_VEC]
     *     Matrix3x3<T> R_mvconvention(N0,N1,N2,angleN0,angleN1,angleN2);
     *     Vector3<T> V(...);
     *     Vector3<T> U = R_mvconvention*V;  // (u0,u1,u2) = R2*R1*R0*V
     *   [GTE_USE_VEC_MAT]
     *     Matrix3x3<T> R_vmconvention(N0,N1,N2,angleN0,angleN1,angleN2);
     *     Vector3<T> V(...);
     *     Vector3<T> U = R_mvconvention*V;  // (u0,u1,u2) = V*R0*R1*R2
     * In either convention, you get the same 3-tuple U.
     *
     * (7) Convert a quaternion to an axis-angle pair, where
     *   q = sin(angle/2)*(axis[0]*i + axis[1]*j + axis[2]*k) + cos(angle/2)
     *
     * (8) Convert an axis-angle pair to a quaternion, where
     *   q = sin(angle/2)*(axis[0]*i + axis[1]*j + axis[2]*k) + cos(angle/2)
     *
     * (9) Convert a quaternion to Euler angles.  The quaternion is converted to
     * a matrix which is then converted to Euler angles.
     *
     * (10) Convert Euler angles to a quaternion.  The Euler angles are converted
     * to a matrix which is then converted to a quaternion.
     *
     * (11) Convert an axis-angle pair to Euler angles.  The axis-angle pair is
     * converted to a quaternion which is then converted to Euler angles.
     *
     * (12) Convert Euler angles to an axis-angle pair.  The Euler angles are
     * converted to a quaternion which is then converted to an axis-angle
     * pair.
     */
    template<typename T, int N, bool _ColumnMajor = true>
    class Rotation
    {
        enum ERepresentation
        {
            MATRIX = 0,
            QUATERNION = 1,
            AXIS_ANGLE = 2,
            EULER_ANGLES = 3
        };
        static const bool CM = _ColumnMajor;
    public:
        Rotation(const Matrix<T, N, N, CM>& matrix);
        Rotation(const Quaternion<T>& quaternion);
        Rotation(const AxisAngle<T, N>& axisAngle);
        Rotation(const EulerAngles<T>& eulerAngles);

        operator Matrix<T, N, N, _ColumnMajor>() const;
        operator   Quaternion<T>() const;
        operator AxisAngle<T, N>() const;
        const EulerAngles<T>& operator()(int i0, int i1, int i2) const;

    private:
        static void Convert(const Matrix<T, N, N, CM>& mat, Quaternion<T>& q);
        static void Convert(const Quaternion<T>& q,         Matrix<T, N, N, CM>& mat);
        static void Convert(const Matrix<T, N, N, CM>& mat, AxisAngle<T, N>& a);
        static void Convert(const AxisAngle<T, N>& a,       Matrix<T, N, N, CM>& mat);
        static void Convert(const Matrix<T, N, N, CM>& mat, EulerAngles<T>& e);
        static void Convert(const EulerAngles<T>& e,        Matrix<T, N, N, CM>& mat);
        static void Convert(const Quaternion<T>& q,         AxisAngle<T, N>& a);
        static void Convert(const AxisAngle<T, N>& a,       Quaternion<T>& q);
        static void Convert(const Quaternion<T>& q,         EulerAngles<T>& e);
        static void Convert(const EulerAngles<T>& e,        Quaternion<T>& q);
        static void Convert(const AxisAngle<T, N>& a,       EulerAngles<T>& e);
        static void Convert(const EulerAngles<T>& e,        AxisAngle<T, N>& a);

    private:
        ERepresentation mRepr;
        mutable bool mIsComputed[4] = { false };
        mutable Matrix<T, N, N, CM> mMatrix;
        mutable Quaternion<T>       mQuaternion;
        mutable AxisAngle<T, N>     mAxisAngle;
        mutable EulerAngles<T>      mEulerAngles;
    };















    /*
                        TEMPLATE CLASS IMPLEMENTATION
      -------------------------------+----------------------------------
                               CLASS : Rotation
                           ARGUMENTS : <typename T, int _NumRows, int _NumCols, bool _ColumnMajor = true>
                         GENERAL IMP : YES
              PARTIAL SPECIALIZATION : NO
                 FULL SPECIALIZATION : NO
      -------------------------------+----------------------------------
    */

    template<typename T, int N, bool _ColumnMajor>
    Rotation<T, N, _ColumnMajor>::Rotation(const Matrix<T, N, N, CM>& matrix)
        : mRepr(ERepresentation::MATRIX)
        , mMatrix(matrix)
    {
        static_assert(N == 3 || N == 4, "Dimension must be 3 or 4.");
        mIsComputed[ERepresentation::MATRIX] = true;
    }


    template<typename T, int N, bool _ColumnMajor>
    Rotation<T, N, _ColumnMajor>::Rotation(const Quaternion<T>& quaternion)
        : mRepr(ERepresentation::QUATERNION)
        , mQuaternion(quaternion)
    {
        static_assert(N == 3 || N == 4, "Dimension must be 3 or 4.");
        mIsComputed[ERepresentation::QUATERNION] = true;
    }


    template<typename T, int N, bool _ColumnMajor>
    Rotation<T, N, _ColumnMajor>::Rotation(const AxisAngle<T, N>& axisAngle)
        : mRepr(ERepresentation::AXIS_ANGLE)
        , mAxisAngle(axisAngle)
    {
        static_assert(N == 3 || N == 4, "Dimension must be 3 or 4.");
        mIsComputed[ERepresentation::AXIS_ANGLE] = true;
    }


    template<typename T, int N, bool _ColumnMajor>
    Rotation<T, N, _ColumnMajor>::Rotation(const EulerAngles<T>& eulerAngles)
        : mRepr(ERepresentation::EULER_ANGLES)
        , mEulerAngles(eulerAngles)
    {
        static_assert(N == 3 || N == 4, "Dimension must be 3 or 4.");
        mIsComputed[ERepresentation::EULER_ANGLES] = true;
    }


    template<typename T, int N, bool _ColumnMajor>
    Rotation<T, N, _ColumnMajor>::operator Matrix<T, N, N, _ColumnMajor>() const
    {
        static_assert(N == 3 || N == 4, "Dimension must be 3 or 4.");

        if (mIsComputed[ERepresentation::MATRIX])
            return mMatrix;

        switch (mRepr) {
        case ERepresentation::MATRIX:
            break;
        case ERepresentation::QUATERNION:
            Convert(mQuaternion, mMatrix);
            break;
        case ERepresentation::AXIS_ANGLE:
            Convert(mAxisAngle, mMatrix);
            break;
        case ERepresentation::EULER_ANGLES:
            Convert(mEulerAngles, mMatrix);
            break;
        default:
            assert(false);
        }

        mIsComputed[ERepresentation::MATRIX] = true;
        return mMatrix;
    }


    template<typename T, int N, bool _ColumnMajor>
    Rotation<T, N, _ColumnMajor>::operator Quaternion<T>() const
    {
        static_assert(N == 3 || N == 4, "Dimension must be 3 or 4.");

        if (mIsComputed[ERepresentation::QUATERNION])
            return mQuaternion;

        switch (mRepr) {
        case ERepresentation::MATRIX:
            Convert(mMatrix, mQuaternion);
            break;
        case ERepresentation::QUATERNION:
            break;
        case ERepresentation::AXIS_ANGLE:
            Convert(mAxisAngle, mQuaternion);
            break;
        case ERepresentation::EULER_ANGLES:
            Convert(mEulerAngles, mQuaternion);
            break;
        default:
            assert(false);
        }

        mIsComputed[ERepresentation::QUATERNION] = true;
        return mQuaternion;
    }


    template<typename T, int N, bool _ColumnMajor>
    Rotation<T, N, _ColumnMajor>::operator AxisAngle<T, N>() const
    {
        static_assert(N == 3 || N == 4, "Dimension must be 3 or 4.");

        if (mIsComputed[ERepresentation::AXIS_ANGLE])
            return mAxisAngle;

        switch (mRepr) {
        case ERepresentation::MATRIX:
            Convert(mMatrix, mAxisAngle);
            break;
        case ERepresentation::QUATERNION:
            Convert(mQuaternion, mAxisAngle);
            break;
        case ERepresentation::AXIS_ANGLE:
            break;
        case ERepresentation::EULER_ANGLES:
            Convert(mEulerAngles, mAxisAngle);
            break;
        default:
            assert(false);
        }

        mIsComputed[ERepresentation::AXIS_ANGLE] = true;
        return mAxisAngle;
    }


    template<typename T, int N, bool _ColumnMajor>
    const EulerAngles<T>& Rotation<T, N, _ColumnMajor>::operator()(int i0, int i1, int i2) const
    {
        static_assert(N == 3 || N == 4, "Dimension must be 3 or 4.");

        mEulerAngles.mAxis[0] = i0;
        mEulerAngles.mAxis[1] = i1;
        mEulerAngles.mAxis[2] = i2;

        switch (mRepr) {
        case ERepresentation::MATRIX:
            Convert(mMatrix, mEulerAngles);
            break;
        case ERepresentation::QUATERNION:
            Convert(mQuaternion, mEulerAngles);
            break;
        case ERepresentation::AXIS_ANGLE:
            Convert(mAxisAngle, mEulerAngles);
            break;
        case ERepresentation::EULER_ANGLES:
            break;
        default:
            assert(false);
        }

        return mEulerAngles;
    }


    template<typename T, int N, bool _ColumnMajor>
    void Rotation<T, N, _ColumnMajor>::Convert(const Matrix<T, N, N, CM>& r, Quaternion<T>& q)
    {
        static_assert(N == 3 || N == 4, "Dimension must be 3 or 4.");

        T r22 = r(2, 2);
        if (r22 <= (T)0)  // x^2 + y^2 >= z^2 + w^2
        {
            T dif10 = r(1, 1) - r(0, 0);
            T omr22 = (T)1 - r22;
            if (dif10 <= (T)0)  // x^2 >= y^2
            {
                T fourXSqr = omr22 - dif10;
                T inv4x = ((T)0.5) / std::sqrt(fourXSqr);
                q[0] = fourXSqr * inv4x;
                q[1] = (r(0, 1) + r(1, 0)) * inv4x;
                q[2] = (r(0, 2) + r(2, 0)) * inv4x;
                if constexpr (Matrix<T, N, N, CM>::IsColumnMajor) {
                    q[3] = (r(2, 1) - r(1, 2)) * inv4x;
                }
                else {
                    q[3] = (r(1, 2) - r(2, 1)) * inv4x;
                }
            }
            else  // y^2 >= x^2
            {
                T fourYSqr = omr22 + dif10;
                T inv4y = ((T)0.5) / std::sqrt(fourYSqr);
                q[0] = (r(0, 1) + r(1, 0)) * inv4y;
                q[1] = fourYSqr * inv4y;
                q[2] = (r(1, 2) + r(2, 1)) * inv4y;
                if constexpr (Matrix<T, N, N, CM>::IsColumnMajor) {
                    q[3] = (r(0, 2) - r(2, 0)) * inv4y;
                }
                else {
                    q[3] = (r(2, 0) - r(0, 2)) * inv4y;
                }
            }
        }
        else  // z^2 + w^2 >= x^2 + y^2
        {
            T sum10 = r(1, 1) + r(0, 0);
            T opr22 = (T)1 + r22;
            if (sum10 <= (T)0)  // z^2 >= w^2
            {
                T fourZSqr = opr22 - sum10;
                T inv4z = ((T)0.5) / std::sqrt(fourZSqr);
                q[0] = (r(0, 2) + r(2, 0)) * inv4z;
                q[1] = (r(1, 2) + r(2, 1)) * inv4z;
                q[2] = fourZSqr * inv4z;
                if constexpr (Matrix<T, N, N, CM>::IsColumnMajor) {
                    q[3] = (r(1, 0) - r(0, 1)) * inv4z;
                }
                else {
                    q[3] = (r(0, 1) - r(1, 0)) * inv4z;
                }
            }
            else  // w^2 >= z^2
            {
                T fourWSqr = opr22 + sum10;
                T inv4w = ((T)0.5) / std::sqrt(fourWSqr);
                if constexpr (Matrix<T, N, N, CM>::IsColumnMajor) {
                    q[0] = (r(2, 1) - r(1, 2)) * inv4w;
                    q[1] = (r(0, 2) - r(2, 0)) * inv4w;
                    q[2] = (r(1, 0) - r(0, 1)) * inv4w;
                }
                else {
                    q[0] = (r(1, 2) - r(2, 1)) * inv4w;
                    q[1] = (r(2, 0) - r(0, 2)) * inv4w;
                    q[2] = (r(0, 1) - r(1, 0)) * inv4w;
                }
                q[3] = fourWSqr * inv4w;
            }
        }
    }



    template<typename T, int N, bool _ColumnMajor>
    void Rotation<T, N, _ColumnMajor>::Convert(const Quaternion<T>& q, Matrix<T, N, N, CM>& r)
    {
        static_assert(N == 3 || N == 4, "Dimension must be 3 or 4.");

        r.MakeIdentity();

        T twoX = ((T)2) * q[0];
        T twoY = ((T)2) * q[1];
        T twoZ = ((T)2) * q[2];
        T twoXX = twoX * q[0];
        T twoXY = twoX * q[1];
        T twoXZ = twoX * q[2];
        T twoXW = twoX * q[3];
        T twoYY = twoY * q[1];
        T twoYZ = twoY * q[2];
        T twoYW = twoY * q[3];
        T twoZZ = twoZ * q[2];
        T twoZW = twoZ * q[3];

        if constexpr (Matrix<T, N, N, CM>::IsColumnMajor) {
            r(0, 0) = (T)1 - twoYY - twoZZ;
            r(0, 1) = twoXY - twoZW;
            r(0, 2) = twoXZ + twoYW;
            r(1, 0) = twoXY + twoZW;
            r(1, 1) = (T)1 - twoXX - twoZZ;
            r(1, 2) = twoYZ - twoXW;
            r(2, 0) = twoXZ - twoYW;
            r(2, 1) = twoYZ + twoXW;
            r(2, 2) = (T)1 - twoXX - twoYY;
        }
        else {
            r(0, 0) = (T)1 - twoYY - twoZZ;
            r(1, 0) = twoXY - twoZW;
            r(2, 0) = twoXZ + twoYW;
            r(0, 1) = twoXY + twoZW;
            r(1, 1) = (T)1 - twoXX - twoZZ;
            r(2, 1) = twoYZ - twoXW;
            r(0, 2) = twoXZ - twoYW;
            r(1, 2) = twoYZ + twoXW;
            r(2, 2) = (T)1 - twoXX - twoYY;
        }
    }



    template<typename T, int N, bool _ColumnMajor>
    void Rotation<T, N, _ColumnMajor>::Convert(const Matrix<T, N, N, CM>& r, AxisAngle<T, N>& a)
    {
        static_assert(N == 3 || N == 4, "Dimension must be 3 or 4.");

        T trace = r(0, 0) + r(1, 1) + r(2, 2);
        T half = (T)0.5;
        T cs = half * (trace - (T)1);
        cs = std::max(std::min(cs, (T)1), (T)-1);
        a.angle = std::acos(cs);  // The angle is in [0,pi].
        a.axis.MakeZero();

        if (a.angle > (T)0)
        {
            if (a.angle < (T)M_PI)
            {
                // The angle is in (0,pi).
                if constexpr (Matrix<T, N, N, CM>::IsColumnMajor) {
                    a.axis[0] = r(2, 1) - r(1, 2);
                    a.axis[1] = r(0, 2) - r(2, 0);
                    a.axis[2] = r(1, 0) - r(0, 1);
                    Normalize(a.axis);
                }
                else {
                    a.axis[0] = r(1, 2) - r(2, 1);
                    a.axis[1] = r(2, 0) - r(0, 2);
                    a.axis[2] = r(0, 1) - r(1, 0);
                    Normalize(a.axis);
                }
            }
            else
            {
                // The angle is pi, in which case R is symmetric and
                // R+I = 2*(I+S^2) = 2*U*U^T, where U = (u0,u1,u2) is the
                // unit-length direction of the rotation axis.  Determine the
                // largest diagonal entry of R+I and normalize the
                // corresponding row to produce U.  It does not matter the
                // sign on u[d] for chosen diagonal d, because R(U,pi) = R(-U,pi).
                T one = (T)1;
                if (r(0, 0) >= r(1, 1))
                {
                    if (r(0, 0) >= r(2, 2))
                    {
                        // r00 is maximum diagonal term
                        a.axis[0] = r(0, 0) + one;
                        a.axis[1] = half * (r(0, 1) + r(1, 0));
                        a.axis[2] = half * (r(0, 2) + r(2, 0));
                    }
                    else
                    {
                        // r22 is maximum diagonal term
                        a.axis[0] = half * (r(2, 0) + r(0, 2));
                        a.axis[1] = half * (r(2, 1) + r(1, 2));
                        a.axis[2] = r(2, 2) + one;
                    }
                }
                else
                {
                    if (r(1, 1) >= r(2, 2))
                    {
                        // r11 is maximum diagonal term
                        a.axis[0] = half * (r(1, 0) + r(0, 1));
                        a.axis[1] = r(1, 1) + one;
                        a.axis[2] = half * (r(1, 2) + r(2, 1));
                    }
                    else
                    {
                        // r22 is maximum diagonal term
                        a.axis[0] = half * (r(2, 0) + r(0, 2));
                        a.axis[1] = half * (r(2, 1) + r(1, 2));
                        a.axis[2] = r(2, 2) + one;
                    }
                }
                Normalize(a.axis);
            }
        }
        else
        {
            // The angle is 0 and the matrix is the identity.  Any axis will
            // work, so choose the Unit(0) axis.
            a.axis[0] = (T)1;
        }
    }




    template<typename T, int N, bool _ColumnMajor>
    void Rotation<T, N, _ColumnMajor>::Convert(const AxisAngle<T, N>& a, Matrix<T, N, N, CM>& r)
    {
        static_assert(N == 3 || N == 4, "Dimension must be 3 or 4.");

        r.MakeIdentity();

        T cs = std::cos(a.angle);
        T sn = std::sin(a.angle);
        T oneMinusCos = ((T)1) - cs;
        T x0sqr = a.axis[0] * a.axis[0];
        T x1sqr = a.axis[1] * a.axis[1];
        T x2sqr = a.axis[2] * a.axis[2];
        T x0x1m = a.axis[0] * a.axis[1] * oneMinusCos;
        T x0x2m = a.axis[0] * a.axis[2] * oneMinusCos;
        T x1x2m = a.axis[1] * a.axis[2] * oneMinusCos;
        T x0Sin = a.axis[0] * sn;
        T x1Sin = a.axis[1] * sn;
        T x2Sin = a.axis[2] * sn;

        if constexpr (Matrix<T, N, N, CM>::IsColumnMajor) {
            r(0, 0) = x0sqr * oneMinusCos + cs;
            r(0, 1) = x0x1m - x2Sin;
            r(0, 2) = x0x2m + x1Sin;
            r(1, 0) = x0x1m + x2Sin;
            r(1, 1) = x1sqr * oneMinusCos + cs;
            r(1, 2) = x1x2m - x0Sin;
            r(2, 0) = x0x2m - x1Sin;
            r(2, 1) = x1x2m + x0Sin;
            r(2, 2) = x2sqr * oneMinusCos + cs;
        }
        else {
            r(0, 0) = x0sqr * oneMinusCos + cs;
            r(1, 0) = x0x1m - x2Sin;
            r(2, 0) = x0x2m + x1Sin;
            r(0, 1) = x0x1m + x2Sin;
            r(1, 1) = x1sqr * oneMinusCos + cs;
            r(2, 1) = x1x2m - x0Sin;
            r(0, 2) = x0x2m - x1Sin;
            r(1, 2) = x1x2m + x0Sin;
            r(2, 2) = x2sqr * oneMinusCos + cs;
        }
    }




    template<typename T, int N, bool _ColumnMajor>
    void Rotation<T, N, _ColumnMajor>::Convert(const Matrix<T, N, N, CM>& r, EulerAngles<T>& e)
    {
        static_assert(N == 3 || N == 4, "Dimension must be 3 or 4.");

        if (0 <= e.axis[0] && e.axis[0] < 3
            && 0 <= e.axis[1] && e.axis[1] < 3
            && 0 <= e.axis[2] && e.axis[2] < 3
            && e.axis[1] != e.axis[0]
            && e.axis[1] != e.axis[2])
        {
            if (e.axis[0] != e.axis[2])
            {
                if constexpr (Matrix<T, N, N, CM>::IsColumnMajor) {
                    // Map (0,1,2), (1,2,0), and (2,0,1) to +1.
                    // Map (0,2,1), (2,1,0), and (1,0,2) to -1.
                    int parity = (((e.axis[2] | (e.axis[1] << 2)) >> e.axis[0]) & 1);
                    T const sgn = (parity & 1 ? (T)-1 : (T)+1);

                    if (r(e.axis[2], e.axis[0]) < (T)1)
                    {
                        if (r(e.axis[2], e.axis[0]) > (T)-1)
                        {
                            e.angle[2] = std::atan2(sgn * r(e.axis[1], e.axis[0]),
                                r(e.axis[0], e.axis[0]));
                            e.angle[1] = std::asin(-sgn * r(e.axis[2], e.axis[0]));
                            e.angle[0] = std::atan2(sgn * r(e.axis[2], e.axis[1]),
                                r(e.axis[2], e.axis[2]));
                            e.result = EulerResult::UNIQUE;
                        }
                        else
                        {
                            e.angle[2] = (T)0;
                            e.angle[1] = sgn * (T)M_PI_2;
                            e.angle[0] = std::atan2(-sgn * r(e.axis[1], e.axis[2]),
                                r(e.axis[1], e.axis[1]));
                            e.result = EulerResult::NOT_UNIQUE_DIF;
                        }
                    }
                    else
                    {
                        e.angle[2] = (T)0;
                        e.angle[1] = -sgn * (T)M_PI_2;
                        e.angle[0] = std::atan2(-sgn * r(e.axis[1], e.axis[2]),
                            r(e.axis[1], e.axis[1]));
                        e.result = EulerResult::NOT_UNIQUE_SUM;
                    }
                }
                else {
                    // Map (0,1,2), (1,2,0), and (2,0,1) to +1.
                    // Map (0,2,1), (2,1,0), and (1,0,2) to -1.
                    int parity = (((e.axis[0] | (e.axis[1] << 2)) >> e.axis[2]) & 1);
                    T const sgn = (parity & 1 ? (T)+1 : (T)-1);

                    if (r(e.axis[0], e.axis[2]) < (T)1)
                    {
                        if (r(e.axis[0], e.axis[2]) > (T)-1)
                        {
                            e.angle[0] = std::atan2(sgn * r(e.axis[1], e.axis[2]),
                                r(e.axis[2], e.axis[2]));
                            e.angle[1] = std::asin(-sgn * r(e.axis[0], e.axis[2]));
                            e.angle[2] = std::atan2(sgn * r(e.axis[0], e.axis[1]),
                                r(e.axis[0], e.axis[0]));
                            e.result = EulerResult::UNIQUE;
                        }
                        else
                        {
                            e.angle[0] = (T)0;
                            e.angle[1] = sgn * (T)M_PI_2;
                            e.angle[2] = std::atan2(-sgn * r(e.axis[1], e.axis[0]),
                                r(e.axis[1], e.axis[1]));
                            e.result = EulerResult::NOT_UNIQUE_DIF;
                        }
                    }
                    else
                    {
                        e.angle[0] = (T)0;
                        e.angle[1] = -sgn * (T)M_PI_2;
                        e.angle[2] = std::atan2(-sgn * r(e.axis[1], e.axis[0]),
                            r(e.axis[1], e.axis[1]));
                        e.result = EulerResult::NOT_UNIQUE_SUM;
                    }
                }
            }
            else
            {
                if constexpr (Matrix<T, N, N, CM>::IsColumnMajor) {
                    // Map (0,2,0), (1,0,1), and (2,1,2) to +1.
                    // Map (0,1,0), (1,2,1), and (2,0,2) to -1.
                    int b0 = 3 - e.axis[1] - e.axis[2];
                    int parity = (((b0 | (e.axis[1] << 2)) >> e.axis[2]) & 1);
                    T const sgn = (parity & 1 ? (T)+1 : (T)-1);

                    if (r(e.axis[2], e.axis[2]) < (T)1)
                    {
                        if (r(e.axis[2], e.axis[2]) > (T)-1)
                        {
                            e.angle[2] = std::atan2(r(e.axis[1], e.axis[2]),
                                sgn * r(b0, e.axis[2]));
                            e.angle[1] = std::acos(r(e.axis[2], e.axis[2]));
                            e.angle[0] = std::atan2(r(e.axis[2], e.axis[1]),
                                -sgn * r(e.axis[2], b0));
                            e.result = EulerResult::UNIQUE;
                        }
                        else
                        {
                            e.angle[2] = (T)0;
                            e.angle[1] = (T)M_PI;
                            e.angle[0] = std::atan2(sgn * r(e.axis[1], b0),
                                r(e.axis[1], e.axis[1]));
                            e.result = EulerResult::NOT_UNIQUE_DIF;
                        }
                    }
                    else
                    {
                        e.angle[2] = (T)0;
                        e.angle[1] = (T)0;
                        e.angle[0] = std::atan2(sgn * r(e.axis[1], b0),
                            r(e.axis[1], e.axis[1]));
                        e.result = EulerResult::NOT_UNIQUE_SUM;
                    }
                }
                else {
                    // Map (0,2,0), (1,0,1), and (2,1,2) to -1.
                    // Map (0,1,0), (1,2,1), and (2,0,2) to +1.
                    int b2 = 3 - e.axis[0] - e.axis[1];
                    int parity = (((b2 | (e.axis[1] << 2)) >> e.axis[0]) & 1);
                    T const sgn = (parity & 1 ? (T)-1 : (T)+1);

                    if (r(e.axis[0], e.axis[0]) < (T)1)
                    {
                        if (r(e.axis[0], e.axis[0]) > (T)-1)
                        {
                            e.angle[0] = std::atan2(r(e.axis[1], e.axis[0]),
                                sgn * r(b2, e.axis[0]));
                            e.angle[1] = std::acos(r(e.axis[0], e.axis[0]));
                            e.angle[2] = std::atan2(r(e.axis[0], e.axis[1]),
                                -sgn * r(e.axis[0], b2));
                            e.result = EulerResult::UNIQUE;
                        }
                        else
                        {
                            e.angle[0] = (T)0;
                            e.angle[1] = (T)M_PI;
                            e.angle[2] = std::atan2(sgn * r(e.axis[1], b2),
                                r(e.axis[1], e.axis[1]));
                            e.result = EulerResult::NOT_UNIQUE_DIF;
                        }
                    }
                    else
                    {
                        e.angle[0] = (T)0;
                        e.angle[1] = (T)0;
                        e.angle[2] = std::atan2(sgn * r(e.axis[1], b2),
                            r(e.axis[1], e.axis[1]));
                        e.result = EulerResult::NOT_UNIQUE_SUM;
                    }
                }
            }
        }
        else
        {
            // Invalid angles.
            e.angle[0] = (T)0;
            e.angle[1] = (T)0;
            e.angle[2] = (T)0;
            e.result = EulerResult::INVALID;
        }
    }





    template<typename T, int N, bool _ColumnMajor>
    void Rotation<T, N, _ColumnMajor>::Convert(const EulerAngles<T>& e, Matrix<T, N, N, CM>& r)
    {
        static_assert(N == 3 || N == 4, "Dimension must be 3 or 4.");

        if (0 <= e.axis[0] && e.axis[0] < 3
            && 0 <= e.axis[1] && e.axis[1] < 3
            && 0 <= e.axis[2] && e.axis[2] < 3
            && e.axis[1] != e.axis[0]
            && e.axis[1] != e.axis[2])
        {
            Matrix<T, N, N, CM> r0, r1, r2;
            Convert(AxisAngle<N, T>(Vector<N, T>::Unit(e.axis[0]),
                e.angle[0]), r0);
            Convert(AxisAngle<N, T>(Vector<N, T>::Unit(e.axis[1]),
                e.angle[1]), r1);
            Convert(AxisAngle<N, T>(Vector<N, T>::Unit(e.axis[2]),
                e.angle[2]), r2);
            if constexpr (Matrix<T, N, N, CM>::IsColumnMajor) {
                r = r2 * r1 * r0;
            }
            else {
                r = r0 * r1 * r2;
            }
        }
        else
        {
            // Invalid angles.
            r.MakeIdentity();
        }
    }




    template<typename T, int N, bool _ColumnMajor>
    void Rotation<T, N, _ColumnMajor>::Convert(const Quaternion<T>& q, AxisAngle<T, N>& a)
    {
        static_assert(N == 3 || N == 4, "Dimension must be 3 or 4.");

        a.axis.MakeZero();

        T axisSqrLen = q[0] * q[0] + q[1] * q[1] + q[2] * q[2];
        if (axisSqrLen > (T)0)
        {
            T adjust = (T)1;
            if constexpr (Matrix<T, N, N, CM>::IsColumnMajor) {
                adjust = ((T)1) / std::sqrt(axisSqrLen);
            }
            else {
                adjust = ((T)-1) / std::sqrt(axisSqrLen);
            }
            a.axis[0] = q[0] * adjust;
            a.axis[1] = q[1] * adjust;
            a.axis[2] = q[2] * adjust;
            T cs = std::max(std::min(q[3], (T)1), (T)-1);
            a.angle = (T)2 * std::acos(cs);
        }
        else
        {
            // The angle is 0 (modulo 2*pi). Any axis will work, so choose the
            // Unit(0) axis.
            a.axis[0] = (T)1;
            a.angle = (T)0;
        }
    }





    template<typename T, int N, bool _ColumnMajor>
    void Rotation<T, N, _ColumnMajor>::Convert(const AxisAngle<T, N>& a, Quaternion<T>& q)
    {
        static_assert(N == 3 || N == 4, "Dimension must be 3 or 4.");

        T halfAngle = (T)1;
        if constexpr (Matrix<T, N, N, CM>::IsColumnMajor) {
            halfAngle = ((T)0.5) * a.angle;
        }
        else {
            halfAngle = ((T)-0.5) * a.angle;
        }
        T sn = std::sin(halfAngle);
        q[0] = sn * a.axis[0];
        q[1] = sn * a.axis[1];
        q[2] = sn * a.axis[2];
        q[3] = std::cos(halfAngle);
    }





    template<typename T, int N, bool _ColumnMajor>
    void Rotation<T, N, _ColumnMajor>::Convert(const Quaternion<T>& q, EulerAngles<T>& e)
    {
        static_assert(N == 3 || N == 4, "Dimension must be 3 or 4.");

        Matrix<T, N, N, CM> r;
        Convert(q, r);
        Convert(r, e);
    }




    template<typename T, int N, bool _ColumnMajor>
    void Rotation<T, N, _ColumnMajor>::Convert(const EulerAngles<T>& e, Quaternion<T>& q)
    {
        static_assert(N == 3 || N == 4, "Dimension must be 3 or 4.");

        Matrix<T, N, N, CM> r;
        Convert(e, r);
        Convert(r, q);
    }




    template<typename T, int N, bool _ColumnMajor>
    void Rotation<T, N, _ColumnMajor>::Convert(const AxisAngle<T, N>& a, EulerAngles<T>& e)
    {
        static_assert(N == 3 || N == 4, "Dimension must be 3 or 4.");

        Quaternion<T> q;
        Convert(a, q);
        Convert(q, e);
    }




    template<typename T, int N, bool _ColumnMajor>
    void Rotation<T, N, _ColumnMajor>::Convert(const EulerAngles<T>& e, AxisAngle<T, N>& a)
    {
        static_assert(N == 3 || N == 4, "Dimension must be 3 or 4.");

        Quaternion<T> q;
        Convert(e, q);
        Convert(q, a);
    }
}