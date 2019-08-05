#pragma once

#include "Geometry/Algebra/Matrix.h"

namespace ZHU
{
    template<typename T, bool CM = true>
    using Matrix4 = Matrix<T, 4, 4, CM>;


    template<typename T, bool CM = true>
    Matrix4<T, CM> Inverse(const Matrix4<T, CM>& M, bool* isInvertible = nullptr);


    template<typename T, bool CM = true>
    Matrix4<T, CM> Adjoint(const Matrix4<T, CM>& M);


    template<typename T, bool CM = true>
    T Determinant(const Matrix4<T, CM>& M);


    template<typename T, bool CM = true>
    T Trace(const Matrix4<T, CM> & M);


    /* The projection plane is Dot(N,X-P) = 0 where N is a 3-by-1 unit-length
     * normal vector and P is a 3-by-1 point on the plane.  The projection is
     * oblique to the plane, in the direction of the 3-by-1 vector D.  Necessarily
     * Dot(N,D) is not zero for this projection to make sense.  Given a 3-by-1
     * point U, compute the intersection of the line U+t*D with the plane to
     * obtain t = -Dot(N,U-P)/Dot(N,D); then
     *
     *   projection(U) = P + [I - D*N^T/Dot(N,D)]*(U-P)
     *
     * A 4-by-4 homogeneous transformation representing the projection is
     *
     *       +-                               -+
     *   M = | D*N^T - Dot(N,D)*I   -Dot(N,P)D |
     *       |          0^T          -Dot(N,D) |
     *       +-                               -+
     *
     * where M applies to [U^T 1]^T by M*[U^T 1]^T.  The matrix is chosen so
     * that M[3][3] > 0 whenever Dot(N,D) < 0; the projection is onto the
     * "positive side" of the plane.
     */
    template<typename T, bool CM = true>
    Matrix4<T, CM> MakeObliqueProjection(const Vector4<T, CM>& origin,
                                         const Vector4<T, CM>& normal,
                                         const Vector4<T, CM>& direction);


    /* The perspective projection of a point onto a plane is
     *
     *     +-                                                 -+
     * M = | Dot(N,E-P)*I - E*N^T    -(Dot(N,E-P)*I - E*N^T)*E |
     *     |        -N^t                      Dot(N,E)         |
     *     +-                                                 -+
     *
     * where E is the eye point, P is a point on the plane, and N is a
     * unit-length plane normal.
     */
    template<typename T, bool CM = true>
    Matrix4<T, CM> MakePerspectiveProjection(const Vector4<T, CM>& origin,
                                             const Vector4<T, CM>& normal,
                                             const Vector4<T, CM>& eye);


    /* The reflection of a point through a plane is
     *     +-                         -+
     * M = | I-2*N*N^T    2*Dot(N,P)*N |
     *     |     0^T            1      |
     *     +-                         -+
     * 
     * where P is a point on the plane and N is a unit-length plane normal.
     */
    template <typename T, bool CM = true>
    Matrix4<T, CM> MakeReflection(const Vector4<T, CM>& origin,
                                  const Vector4<T, CM>& normal);















    /*
                        TEMPLATE CLASS IMPLEMENTATION
      -------------------------------+----------------------------------
                            FUNCTION : for Matrix4<T>
                           ARGUMENTS : <typename T>
                         GENERAL IMP : YES
              PARTIAL SPECIALIZATION : NO
                 FULL SPECIALIZATION : NO
      -------------------------------+----------------------------------
    */

    template<typename T, bool CM>
    Matrix4<T, CM> Inverse(const Matrix4<T, CM>& M, bool* isInvertible)
    {
        Matrix4<T, CM> inverse;
        bool invertible;
        T a0 = M(0, 0) * M(1, 1) - M(0, 1) * M(1, 0);
        T a1 = M(0, 0) * M(1, 2) - M(0, 2) * M(1, 0);
        T a2 = M(0, 0) * M(1, 3) - M(0, 3) * M(1, 0);
        T a3 = M(0, 1) * M(1, 2) - M(0, 2) * M(1, 1);
        T a4 = M(0, 1) * M(1, 3) - M(0, 3) * M(1, 1);
        T a5 = M(0, 2) * M(1, 3) - M(0, 3) * M(1, 2);
        T b0 = M(2, 0) * M(3, 1) - M(2, 1) * M(3, 0);
        T b1 = M(2, 0) * M(3, 2) - M(2, 2) * M(3, 0);
        T b2 = M(2, 0) * M(3, 3) - M(2, 3) * M(3, 0);
        T b3 = M(2, 1) * M(3, 2) - M(2, 2) * M(3, 1);
        T b4 = M(2, 1) * M(3, 3) - M(2, 3) * M(3, 1);
        T b5 = M(2, 2) * M(3, 3) - M(2, 3) * M(3, 2);
        T det = a0 * b5 - a1 * b4 + a2 * b3 + a3 * b2 - a4 * b1 + a5 * b0;
        if (det != (T)0) {
            T invDet = ((T)1) / det;
            inverse = Matrix4<T, CM>
            {
                (+M(1, 1) * b5 - M(1, 2) * b4 + M(1, 3) * b3) * invDet,
                (-M(0, 1) * b5 + M(0, 2) * b4 - M(0, 3) * b3) * invDet,
                (+M(3, 1) * a5 - M(3, 2) * a4 + M(3, 3) * a3) * invDet,
                (-M(2, 1) * a5 + M(2, 2) * a4 - M(2, 3) * a3) * invDet,
                (-M(1, 0) * b5 + M(1, 2) * b2 - M(1, 3) * b1) * invDet,
                (+M(0, 0) * b5 - M(0, 2) * b2 + M(0, 3) * b1) * invDet,
                (-M(3, 0) * a5 + M(3, 2) * a2 - M(3, 3) * a1) * invDet,
                (+M(2, 0) * a5 - M(2, 2) * a2 + M(2, 3) * a1) * invDet,
                (+M(1, 0) * b4 - M(1, 1) * b2 + M(1, 3) * b0) * invDet,
                (-M(0, 0) * b4 + M(0, 1) * b2 - M(0, 3) * b0) * invDet,
                (+M(3, 0) * a4 - M(3, 1) * a2 + M(3, 3) * a0) * invDet,
                (-M(2, 0) * a4 + M(2, 1) * a2 - M(2, 3) * a0) * invDet,
                (-M(1, 0) * b3 + M(1, 1) * b1 - M(1, 2) * b0) * invDet,
                (+M(0, 0) * b3 - M(0, 1) * b1 + M(0, 2) * b0) * invDet,
                (-M(3, 0) * a3 + M(3, 1) * a1 - M(3, 2) * a0) * invDet,
                (+M(2, 0) * a3 - M(2, 1) * a1 + M(2, 2) * a0) * invDet
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
    Matrix4<T, CM> Adjoint(const Matrix4<T, CM>& M)
    {
        T a0 = M(0, 0) * M(1, 1) - M(0, 1) * M(1, 0);
        T a1 = M(0, 0) * M(1, 2) - M(0, 2) * M(1, 0);
        T a2 = M(0, 0) * M(1, 3) - M(0, 3) * M(1, 0);
        T a3 = M(0, 1) * M(1, 2) - M(0, 2) * M(1, 1);
        T a4 = M(0, 1) * M(1, 3) - M(0, 3) * M(1, 1);
        T a5 = M(0, 2) * M(1, 3) - M(0, 3) * M(1, 2);
        T b0 = M(2, 0) * M(3, 1) - M(2, 1) * M(3, 0);
        T b1 = M(2, 0) * M(3, 2) - M(2, 2) * M(3, 0);
        T b2 = M(2, 0) * M(3, 3) - M(2, 3) * M(3, 0);
        T b3 = M(2, 1) * M(3, 2) - M(2, 2) * M(3, 1);
        T b4 = M(2, 1) * M(3, 3) - M(2, 3) * M(3, 1);
        T b5 = M(2, 2) * M(3, 3) - M(2, 3) * M(3, 2);

        return Matrix4<T, CM>
        {
            +M(1, 1) * b5 - M(1, 2) * b4 + M(1, 3) * b3,
            -M(0, 1) * b5 + M(0, 2) * b4 - M(0, 3) * b3,
            +M(3, 1) * a5 - M(3, 2) * a4 + M(3, 3) * a3,
            -M(2, 1) * a5 + M(2, 2) * a4 - M(2, 3) * a3,
            -M(1, 0) * b5 + M(1, 2) * b2 - M(1, 3) * b1,
            +M(0, 0) * b5 - M(0, 2) * b2 + M(0, 3) * b1,
            -M(3, 0) * a5 + M(3, 2) * a2 - M(3, 3) * a1,
            +M(2, 0) * a5 - M(2, 2) * a2 + M(2, 3) * a1,
            +M(1, 0) * b4 - M(1, 1) * b2 + M(1, 3) * b0,
            -M(0, 0) * b4 + M(0, 1) * b2 - M(0, 3) * b0,
            +M(3, 0) * a4 - M(3, 1) * a2 + M(3, 3) * a0,
            -M(2, 0) * a4 + M(2, 1) * a2 - M(2, 3) * a0,
            -M(1, 0) * b3 + M(1, 1) * b1 - M(1, 2) * b0,
            +M(0, 0) * b3 - M(0, 1) * b1 + M(0, 2) * b0,
            -M(3, 0) * a3 + M(3, 1) * a1 - M(3, 2) * a0,
            +M(2, 0) * a3 - M(2, 1) * a1 + M(2, 2) * a0
        };
    }


    template<typename T, bool CM>
    T Determinant(const Matrix4<T, CM> & M)
    {
        T a0 = M(0, 0) * M(1, 1) - M(0, 1) * M(1, 0);
        T a1 = M(0, 0) * M(1, 2) - M(0, 2) * M(1, 0);
        T a2 = M(0, 0) * M(1, 3) - M(0, 3) * M(1, 0);
        T a3 = M(0, 1) * M(1, 2) - M(0, 2) * M(1, 1);
        T a4 = M(0, 1) * M(1, 3) - M(0, 3) * M(1, 1);
        T a5 = M(0, 2) * M(1, 3) - M(0, 3) * M(1, 2);
        T b0 = M(2, 0) * M(3, 1) - M(2, 1) * M(3, 0);
        T b1 = M(2, 0) * M(3, 2) - M(2, 2) * M(3, 0);
        T b2 = M(2, 0) * M(3, 3) - M(2, 3) * M(3, 0);
        T b3 = M(2, 1) * M(3, 2) - M(2, 2) * M(3, 1);
        T b4 = M(2, 1) * M(3, 3) - M(2, 3) * M(3, 1);
        T b5 = M(2, 2) * M(3, 3) - M(2, 3) * M(3, 2);
        T det = a0 * b5 - a1 * b4 + a2 * b3 + a3 * b2 - a4 * b1 + a5 * b0;
        return det;
    }


    template<typename T, bool CM>
    T Trace(const Matrix4<T, CM> & M)
    {
        T trace = M(0, 0) + M(1, 1) + M(2, 2) + M(3, 3);
        return trace;
    }



    template<typename T, bool CM>
    Matrix4<T, CM> MakeObliqueProjection(const Vector4<T, CM>& origin,
                                         const Vector4<T, CM>& normal,
                                         const Vector4<T, CM>& direction)
    {
        Matrix4<T, CM> M;

        T const zero = (T)0;
        T dotND = Dot(normal, direction);
        T dotNO = Dot(origin, normal);

        if constexpr (Matrix4<T, CM>::IsColumnMajor) {
            M(0, 0) = direction[0] * normal[0] - dotND;
            M(0, 1) = direction[0] * normal[1];
            M(0, 2) = direction[0] * normal[2];
            M(0, 3) = -dotNO * direction[0];
            M(1, 0) = direction[1] * normal[0];
            M(1, 1) = direction[1] * normal[1] - dotND;
            M(1, 2) = direction[1] * normal[2];
            M(1, 3) = -dotNO * direction[1];
            M(2, 0) = direction[2] * normal[0];
            M(2, 1) = direction[2] * normal[1];
            M(2, 2) = direction[2] * normal[2] - dotND;
            M(2, 3) = -dotNO * direction[2];
            M(3, 0) = zero;
            M(3, 1) = zero;
            M(3, 2) = zero;
            M(3, 3) = -dotND;
        }
        else {
            M(0, 0) = direction[0] * normal[0] - dotND;
            M(1, 0) = direction[0] * normal[1];
            M(2, 0) = direction[0] * normal[2];
            M(3, 0) = -dotNO * direction[0];
            M(0, 1) = direction[1] * normal[0];
            M(1, 1) = direction[1] * normal[1] - dotND;
            M(2, 1) = direction[1] * normal[2];
            M(3, 1) = -dotNO * direction[1];
            M(0, 2) = direction[2] * normal[0];
            M(1, 2) = direction[2] * normal[1];
            M(2, 2) = direction[2] * normal[2] - dotND;
            M(3, 2) = -dotNO * direction[2];
            M(0, 2) = zero;
            M(1, 3) = zero;
            M(2, 3) = zero;
            M(3, 3) = -dotND;
        }

        return M;
    }



    template<typename T, bool CM>
    Matrix4<T, CM> MakePerspectiveProjection(const Vector4<T, CM>& origin,
                                             const Vector4<T, CM>& normal,
                                             const Vector4<T, CM>& eye)
    {
        Matrix4<T, CM> M;

        T dotND = Dot(normal, eye - origin);

        if constexpr (Matrix4<T, CM>::IsColumnMajor) {
            M(0, 0) = dotND - eye[0] * normal[0];
            M(0, 1) = -eye[0] * normal[1];
            M(0, 2) = -eye[0] * normal[2];
            M(0, 3) = -(M(0, 0) * eye[0] + M(0, 1) * eye[1] + M(0, 2) * eye[2]);
            M(1, 0) = -eye[1] * normal[0];
            M(1, 1) = dotND - eye[1] * normal[1];
            M(1, 2) = -eye[1] * normal[2];
            M(1, 3) = -(M(1, 0) * eye[0] + M(1, 1) * eye[1] + M(1, 2) * eye[2]);
            M(2, 0) = -eye[2] * normal[0];
            M(2, 1) = -eye[2] * normal[1];
            M(2, 2) = dotND - eye[2] * normal[2];
            M(2, 3) = -(M(2, 0) * eye[0] + M(2, 1) * eye[1] + M(2, 2) * eye[2]);
            M(3, 0) = -normal[0];
            M(3, 1) = -normal[1];
            M(3, 2) = -normal[2];
            M(3, 3) = Dot(eye, normal);
        }
        else {
            M(0, 0) = dotND - eye[0] * normal[0];
            M(1, 0) = -eye[0] * normal[1];
            M(2, 0) = -eye[0] * normal[2];
            M(3, 0) = -(M(0, 0) * eye[0] + M(0, 1) * eye[1] + M(0, 2) * eye[2]);
            M(0, 1) = -eye[1] * normal[0];
            M(1, 1) = dotND - eye[1] * normal[1];
            M(2, 1) = -eye[1] * normal[2];
            M(3, 1) = -(M(1, 0) * eye[0] + M(1, 1) * eye[1] + M(1, 2) * eye[2]);
            M(0, 2) = -eye[2] * normal[0];
            M(1, 2) = -eye[2] * normal[1];
            M(2, 2) = dotND - eye[2] * normal[2];
            M(3, 2) = -(M(2, 0) * eye[0] + M(2, 1) * eye[1] + M(2, 2) * eye[2]);
            M(0, 3) = -normal[0];
            M(1, 3) = -normal[1];
            M(2, 3) = -normal[2];
            M(3, 3) = Dot(eye, normal);
        }

        return M;
    }


    /* The reflection of a point through a plane is
     *     +-                         -+
     * M = | I-2*N*N^T    2*Dot(N,P)*N |
     *     |     0^T            1      |
     *     +-                         -+
     *
     * where P is a point on the plane and N is a unit-length plane normal.
     */
    template <typename T, bool CM>
    Matrix4<T, CM> MakeReflection(const Vector4<T, CM>& origin,
                                  const Vector4<T, CM>& normal)
    {
        Matrix4<T, CM> M;

        T const zero = (T)0, one = (T)1, two = (T)2;
        T twoDotNO = two * Dot(origin, normal);

        if constexpr (Matrix4<T, CM>::IsColumnMajor) {
            M(0, 0) = one - two * normal[0] * normal[0];
            M(0, 1) = -two * normal[0] * normal[1];
            M(0, 2) = -two * normal[0] * normal[2];
            M(0, 3) = twoDotNO * normal[0];
            M(1, 0) = M(0, 1);
            M(1, 1) = one - two * normal[1] * normal[1];
            M(1, 2) = -two * normal[1] * normal[2];
            M(1, 3) = twoDotNO * normal[1];
            M(2, 0) = M(0, 2);
            M(2, 1) = M(1, 2);
            M(2, 2) = one - two * normal[2] * normal[2];
            M(2, 3) = twoDotNO * normal[2];
            M(3, 0) = zero;
            M(3, 1) = zero;
            M(3, 2) = zero;
            M(3, 3) = one;
        }
        else {
            M(0, 0) = one - two * normal[0] * normal[0];
            M(1, 0) = -two * normal[0] * normal[1];
            M(2, 0) = -two * normal[0] * normal[2];
            M(3, 0) = twoDotNO * normal[0];
            M(0, 1) = M(1, 0);
            M(1, 1) = one - two * normal[1] * normal[1];
            M(2, 1) = -two * normal[1] * normal[2];
            M(3, 1) = twoDotNO * normal[1];
            M(0, 2) = M(2, 0);
            M(1, 2) = M(2, 1);
            M(2, 2) = one - two * normal[2] * normal[2];
            M(3, 2) = twoDotNO * normal[2];
            M(0, 3) = zero;
            M(1, 3) = zero;
            M(2, 3) = zero;
            M(3, 3) = one;
        }

        return M;
    }
}