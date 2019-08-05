#pragma once

#include "Geometry/Algebra/Matrix.h"

namespace ZHU
{
    template<typename T, bool CM = true>
    using Matrix2 = Matrix<T, 2, 2, CM>;


    /* Create a rotation matrix from an angle (in radians).  The matrix is
     * [GTE_USE_MAT_VEC]
     *   R(t) = {{c,-s},{s,c}}
     * [GTE_USE_VEC_MAT]
     *   R(t) = {{c,s},{-s,c}}
     * where c = cos(t), s = sin(t), and the inner-brace pairs are rows of the matrix.
     */
    template <typename T, bool CM = true>
    void MakeRotation(T angle, Matrix2<T, CM>& rotation);


    /* Get the angle (radians) from a rotation matrix.  
     * The caller is responsible for ensuring the matrix is a rotation.
     */
    template <typename T, bool CM = true>
    T GetRotationAngle(const Matrix2<T, CM>& rotation);


    template <typename T, bool CM = true>
    Matrix2<T, CM> Inverse(const Matrix2<T, CM>& M, bool* isInvertible = nullptr);


    template <typename T, bool CM = true>
    Matrix2<T, CM> Adjoint(const Matrix2<T, CM>& M);


    template <typename T, bool CM = true>
    T Determinant(const Matrix2<T, CM>& M);


    template <typename T, bool CM = true>
    T Trace(const Matrix2<T, CM>& M);















    /*
                        TEMPLATE CLASS IMPLEMENTATION
      -------------------------------+----------------------------------
                            FUNCTION : for Matrix2<T>
                           ARGUMENTS : <typename T>
                         GENERAL IMP : YES
              PARTIAL SPECIALIZATION : NO
                 FULL SPECIALIZATION : NO
      -------------------------------+----------------------------------
    */

    template <typename T, bool CM = true>
    void MakeRotation(T angle, Matrix2<T, CM>& rotation)
    {
        T cs = std::cos(angle);
        T sn = std::sin(angle);

        if constexpr (Matrix2<T, CM>::IsColumnMajor) {
            rotation(0, 0) = cs;
            rotation(0, 1) = -sn;
            rotation(1, 0) = sn;
            rotation(1, 1) = cs;
        }
        else {
            rotation(0, 0) = cs;
            rotation(0, 1) = sn;
            rotation(1, 0) = -sn;
            rotation(1, 1) = cs;
        }
    }


    /* Get the angle (radians) from a rotation matrix.
     * The caller is responsible for ensuring the matrix is a rotation.
     */
    template <typename T, bool CM = true>
    T GetRotationAngle(const Matrix2<T, CM>& rotation)
    {
        if constexpr (Matrix2<T, CM>::IsColumnMajor) {
             return std::atan2(rotation(1, 0), rotation(0, 0));
        }
        else {
            return std::atan2(rotation(0, 1), rotation(0, 0));
        }
    }


    template <typename T, bool CM = true>
    Matrix2<T> Inverse(const Matrix2<T, CM>& M, bool* isInvertible)
    {
        Matrix2<T, CM> inverse;
        bool invertible;
        T det = M(0, 0) * M(1, 1) - M(0, 1) * M(1, 0);
        if (det != (T)0) {
            T invDet = ((T)1) / det;
            inverse = Matrix2<T, CM>
            {
                M(1, 1) * invDet, -M(0, 1) * invDet,
               -M(1, 0) * invDet, M(0, 0) * invDet
            };
            invertible = true;
        }
        else {
            inverse.MakeZero();
            invertible = false;
        }

        if (isInvertible) {
            *isInvertible = invertible;
        }
        return inverse;
    }


    template <typename T, bool CM = true>
    Matrix2<T> Adjoint(const Matrix2<T, CM>& M)
    {
        return Matrix2<T, CM>
        {
            M(1, 1), -M(0, 1),
           -M(1, 0),  M(0, 0)
        };
    }


    template <typename T, bool CM = true>
    T Determinant(const Matrix2<T, CM>& M)
    {
        T det = M(0, 0) * M(1, 1) - M(0, 1) * M(1, 0);
        return det;
    }


    template <typename T, bool CM = true>
    T Trace(const Matrix2<T, CM>& M)
    {
        T trace = M(0, 0) + M(1, 1);
        return trace;
    }
}