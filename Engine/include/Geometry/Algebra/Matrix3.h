#pragma once

#include "Geometry/Algebra/Matrix.h"

namespace ZHU
{
    template<typename T, bool CM = true>
    using Matrix3 = Matrix<T, 3, 3, CM>;


    template<typename T, bool CM = true>
    Matrix3<T, CM> Inverse(const Matrix3<T, CM>& M, bool* isInvertible = nullptr);


    template<typename T, bool CM = true>
    Matrix3<T, CM> Adjoint(const Matrix3<T, CM>& M);


    template<typename T, bool CM = true>
    T Determinant(const Matrix3<T, CM>& M);


    template<typename T, bool CM = true>
    T Trace(const Matrix3<T, CM>& M);















    /*
                        TEMPLATE CLASS IMPLEMENTATION
      -------------------------------+----------------------------------
                            FUNCTION : for Matrix3<T>
                           ARGUMENTS : <typename T>
                         GENERAL IMP : YES
              PARTIAL SPECIALIZATION : NO
                 FULL SPECIALIZATION : NO
      -------------------------------+----------------------------------
    */

    template<typename T, bool CM>
    Matrix3<T, CM> Inverse(const Matrix3<T, CM>& M, bool* isInvertible)
    {
        Matrix3<T, CM> inverse;
        bool invertible;
        T c00 = M(1, 1) * M(2, 2) - M(1, 2) * M(2, 1);
        T c10 = M(1, 2) * M(2, 0) - M(1, 0) * M(2, 2);
        T c20 = M(1, 0) * M(2, 1) - M(1, 1) * M(2, 0);
        T det = M(0, 0) * c00 + M(0, 1) * c10 + M(0, 2) * c20;
        if (det != (T)0) {
            T invDet = ((T)1) / det;
            inverse = Matrix3<T, CM>
            {
                c00 * invDet,
                    (M(0, 2) * M(2, 1) - M(0, 1) * M(2, 2)) * invDet,
                    (M(0, 1) * M(1, 2) - M(0, 2) * M(1, 1)) * invDet,
                    c10 * invDet,
                    (M(0, 0) * M(2, 2) - M(0, 2) * M(2, 0)) * invDet,
                    (M(0, 2) * M(1, 0) - M(0, 0) * M(1, 2)) * invDet,
                    c20 * invDet,
                    (M(0, 1) * M(2, 0) - M(0, 0) * M(2, 1)) * invDet,
                    (M(0, 0) * M(1, 1) - M(0, 1) * M(1, 0)) * invDet
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


    template<typename T, bool CM>
    Matrix3<T, CM> Adjoint(const Matrix3<T, CM>& M)
    {
        return Matrix3<T, CM>
        {
            M(1, 1)* M(2, 2) - M(1, 2) * M(2, 1),
            M(0, 2)* M(2, 1) - M(0, 1) * M(2, 2),
            M(0, 1)* M(1, 2) - M(0, 2) * M(1, 1),
            M(1, 2)* M(2, 0) - M(1, 0) * M(2, 2),
            M(0, 0)* M(2, 2) - M(0, 2) * M(2, 0),
            M(0, 2)* M(1, 0) - M(0, 0) * M(1, 2),
            M(1, 0)* M(2, 1) - M(1, 1) * M(2, 0),
            M(0, 1)* M(2, 0) - M(0, 0) * M(2, 1),
            M(0, 0)* M(1, 1) - M(0, 1) * M(1, 0)
        };
    }


    template<typename T, bool CM>
    T Determinant(const Matrix3<T, CM> & M)
    {
        T c00 = M(1, 1) * M(2, 2) - M(1, 2) * M(2, 1);
        T c10 = M(1, 2) * M(2, 0) - M(1, 0) * M(2, 2);
        T c20 = M(1, 0) * M(2, 1) - M(1, 1) * M(2, 0);
        T det = M(0, 0) * c00 + M(0, 1) * c10 + M(0, 2) * c20;
        return det;
    }


    template<typename T, bool CM>
    T Trace(const Matrix3<T, CM> & M)
    {
        T trace = M(0, 0) + M(1, 1) + M(2, 2);
        return trace;
    }
}