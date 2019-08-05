#pragma once

#include "Geometry/Algebra/Vector.h"

namespace ZHU
{
    template<typename T, bool CM = true>
    using Vector3 = Vector<T, 3, CM>;

    /*
     * Cross, UnitCross, and DotCross have a template parameter N that should
     * be 3 or 4.  The latter case supports affine vectors in 4D (last component
     * w = 0) when you want to use 4-tuples and 4x4 matrices for affine algebra.
     */

     /* Compute the cross product using the formal determinant:
      *   cross = det{{e0,e1,e2},{x0,x1,x2},{y0,y1,y2}}
      *         = (x1*y2-x2*y1, x2*y0-x0*y2, x0*y1-x1*y0)
      * where e0 = (1,0,0), e1 = (0,1,0), e2 = (0,0,1), v0 = (x0,x1,x2), and v1 = (y0,y1,y2).
      */
    template<typename T, int N, bool CM = true, 
             typename = std::enable_if_t<N==3 || N==4>>
    Vector<T, N, CM> Cross(const Vector<T, N, CM>& v0, const Vector<T, N, CM> & v1);


    // Compute the normalized cross product.
    template<typename T, int N, bool CM = true,
             typename = std::enable_if_t<N == 3 || N == 4>>
    Vector<T, N, CM> UnitCross(const Vector<T, N, CM>& v0, const Vector<T, N, CM>& v1,
                               bool robust = false);


    /* Compute Dot((x0,x1,x2),Cross((y0,y1,y2),(z0,z1,z2)), the triple scalar
     * product of three vectors, where v0 = (x0,x1,x2), v1 = (y0,y1,y2), and v2 is (z0,z1,z2).
     *
     * = Dot(v0, Cross(v1, v2))
     */
    template<typename T, int N, bool CM = true,
             typename = std::enable_if_t<N == 3 || N == 4>>
    T DotCross(const Vector<T, N, CM>& v0,
               const Vector<T, N, CM>& v1,
               const Vector<T, N, CM>& v2);


    /* Compute a right-handed orthonormal basis for the orthogonal complement
     * of the input vectors.  The function returns the smallest length of the
     * unnormalized vectors computed during the process.  If this value is nearly
     * zero, it is possible that the inputs are linearly dependent (within
     * numerical round-off errors).  On input, numInputs must be 1 or 2 and
     * v[0] through v[numInputs-1] must be initialized.  On output, the
     * vectors v[0] through v[2] form an orthonormal set.
     */
    template<typename T, bool CM = true>
    T ComputeOrthoComplement(int numInputs, Vector3<T, CM>* v,
                             bool robust = false);


    /* Compute the barycentric coordinates of the point P with respect to the
     * tetrahedron <V0,V1,V2,V3>, P = b0*V0 + b1*V1 + b2*V2 + b3*V3, where
     * b0 + b1 + b2 + b3 = 1.  The return value is 'true' iff {V0,V1,V2,V3} is
     * a linearly independent set.  Numerically, this is measured by
     * |det[V0 V1 V2 V3]| <= epsilon.  The values bary[] are valid only when the
     * return value is 'true' but set to zero when the return value is 'false'.
     */
    template<typename T, bool CM = true>
    bool ComputeBarycentrics(const Vector3<T, CM>& p,
                             const Vector3<T, CM>& v0,
                             const Vector3<T, CM>& v1,
                             const Vector3<T, CM>& v2,
                             const Vector3<T, CM>& v3,
                             T bary[4],
                             T epsilon = (T)0);















    /*
                        TEMPLATE CLASS IMPLEMENTATION
      -------------------------------+----------------------------------
                            FUNCTION : for Vector3<>
                           ARGUMENTS : <typename T>
                         GENERAL IMP : YES
              PARTIAL SPECIALIZATION : NO
                 FULL SPECIALIZATION : NO
      -------------------------------+----------------------------------
    */

    template<typename T, int N, bool CM, typename>
    Vector<T, N, CM> Cross(const Vector<T, N, CM> & v0, const Vector<T, N, CM> & v1)
    {
        static_assert(N == 3 || N == 4, "Dimension must be 3 or 4.");

        Vector<T, N, CM> result;
        result.MakeZero();
        result[0] = v0[1] * v1[2] - v0[2] * v1[1];
        result[1] = v0[2] * v1[0] - v0[0] * v1[2];
        result[2] = v0[0] * v1[1] - v0[1] * v1[0];
        return result;
    }


    template<typename T, int N, bool CM, typename>
    Vector<T, N, CM> UnitCross(const Vector<T, N, CM> & v0, const Vector<T, N, CM> & v1,
                           bool robust)
    {
        static_assert(N == 3 || N == 4, "Dimension must be 3 or 4.");

        Vector<T, N, CM> unitCross = Cross(v0, v1);
        Normalize(unitCross, robust);
        return unitCross;
    }


    template<typename T, int N, bool CM, typename>
    T DotCross(const Vector<T, N, CM> & v0, const Vector<T, N, CM> & v1, const Vector<T, N, CM> & v2)
    {
        static_assert(N == 3 || N == 4, "Dimension must be 3 or 4.");

        return Dot(v0, Cross(v1, v2));
    }


    template<typename T, bool CM>
    T ComputeOrthoComplement(int numInputs, Vector3<T, CM>* v,
                             bool robust)
    {
        if (numInputs == 1) {
            if (std::abs(v[0][0]) > std::abs(v[0][1])) {
                v[1] = { -v[0][2], (T)0, +v[0][0] };
            }
            else {
                v[1] = { (T)0, +v[0][2], -v[0][1] };
            }
            numInputs = 2;
        }

        if (numInputs == 2) {
            v[2] = Cross(v[0], v[1]);
            return Orthonormalize<3, T, CM>(3, v, robust);
        }

        return (T)0;
    }



    template<typename T, bool CM>
    bool ComputeBarycentrics(const Vector3<T, CM>& p,
                             const Vector3<T, CM>& v0,
                             const Vector3<T, CM>& v1,
                             const Vector3<T, CM>& v2,
                             const Vector3<T, CM>& v3,
                             T bary[4],
                             T epsilon)
    {
        // Compute the vectors relative to V3 of the tetrahedron.
        Vector3<T, CM> diff[4] = { v0 - v3, v1 - v3, v2 - v3, p - v3 };

        T det = DotCross(diff[0], diff[1], diff[2]);
        if (det < -epsilon || det > epsilon) {
            T invDet = ((T)1) / det;
            bary[0] = DotCross(diff[3], diff[1], diff[2]) * invDet;
            bary[1] = DotCross(diff[3], diff[2], diff[0]) * invDet;
            bary[2] = DotCross(diff[3], diff[0], diff[1]) * invDet;
            bary[3] = (T)1 - bary[0] - bary[1] - bary[2];
            return true;
        }

        for (int i = 0; i < 4; ++i) {
            bary[i] = (T)0;
        }
        return false;
    }
}