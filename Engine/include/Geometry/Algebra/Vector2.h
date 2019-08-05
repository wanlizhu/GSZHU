#pragma once

#include "Geometry/Algebra/Vector.h"

namespace ZHU
{
    template<typename T, bool CM = true>
    using Vector2 = Vector<T, 2, CM>;


    /* Compute the perpendicular using the formal determinant,
     * perp = det{{e0,e1},{x0,x1}} = (x1,-x0)
     * where e0 = (1,0), e1 = (0,1), and v = (x0,x1).
     */
    template<typename T, bool CM = true>
    Vector2<T, CM> Perp(const Vector2<T>& v);


    // Compute the normalized perpendicular.
    template <typename T, bool CM = true>
    Vector2<T, CM> UnitPerp(const Vector2<T, CM>& v, bool robust = false);


    // Compute Dot((x0,x1),Perp(y0,y1)) = x0*y1 - x1*y0, 
    // where v0 = (x0,x1) and v1 = (y0,y1).
    template <typename T, bool CM = true>
    T DotPerp(Vector2<T, CM> const& v0, Vector2<T, CM> const& v1);


    /* Compute a right-handed orthonormal basis 
     * for the orthogonal complement of the input vectors.  
     * The function returns the smallest length of the
     * unnormalized vectors computed during the process.  If this value is nearly
     * zero, it is possible that the inputs are linearly dependent (within
     * numerical round-off errors).  On input, numInputs must be 1 and v[0]
     * must be initialized.  On output, the vectors v[0] and v[1] form an
     * orthonormal set.
     */
    template<typename T, bool CM = true>
    T ComputeOrthoComplement(int numInputs, Vector2<T, CM>* v, bool robust = false);


    /* Compute the barycentric coordinates of the point P with respect to the
     * triangle <V0,V1,V2>, P = b0*V0 + b1*V1 + b2*V2, where b0 + b1 + b2 = 1.
     * The return value is 'true' iff {V0,V1,V2} is a linearly independent set.
     * Numerically, this is measured by |det[V0 V1 V2]| <= epsilon.  The values
     * bary[] are valid only when the return value is 'true' but set to zero when
     * the return value is 'false'.
     */
    template<typename T, bool CM = true>
    bool ComputeBarycentrics(const Vector2<T, CM>& p,
                             const Vector2<T, CM>& v0,
                             const Vector2<T, CM>& v1,
                             const Vector2<T, CM>& v2,
                             T bary[3],
                             T epsilon = (T)0);















    /*
                        TEMPLATE CLASS IMPLEMENTATION
      -------------------------------+----------------------------------
                            FUNCTION : for Vector2<T>
                           ARGUMENTS : <typename T>
                         GENERAL IMP : YES
              PARTIAL SPECIALIZATION : NO
                 FULL SPECIALIZATION : NO
      -------------------------------+----------------------------------
    */

    template <typename T, bool CM>
    Vector2<T, CM> Perp(const Vector2<T, CM>& v)
    {
        return Vector2<T, CM>{ v[1], -v[0] };
    }


    template <typename T, bool CM>
    Vector2<T, CM> UnitPerp(const Vector2<T, CM>& v, bool robust)
    {
        Vector2<T, CM> unitPerp{ v[1], -v[0] };
        Normalize(unitPerp, robust);
        return unitPerp;
    }


    template <typename T, bool CM>
    T DotPerp(const Vector2<T, CM>& v0, const Vector2<T, CM>& v1)
    {
        return Dot(v0, Perp(v1));
    }


    template <typename T, bool CM>
    T ComputeOrthoComplement(int numInputs, Vector2<T, CM>* v, bool robust)
    {
        if (numInputs == 1) {
            v[1] = -Perp(v[0]);
            return Orthonormalize<T, 2, CM>(2, v, robust);
        }

        return (T)0;
    }


    template <typename T, bool CM>
    bool ComputeBarycentrics(const Vector2<T, CM>& p,
                             const Vector2<T, CM>& v0,
                             const Vector2<T, CM>& v1, 
                             const Vector2<T, CM>& v2, 
                             T bary[3],
                             T epsilon)
    {
        // Compute the vectors relative to V2 of the triangle.
        Vector2<T, CM> diff[3] = { v0 - v2, v1 - v2, p - v2 };

        T det = DotPerp(diff[0], diff[1]);
        if (det < -epsilon || det > epsilon) {
            T invDet = ((T)1) / det;
            bary[0] = DotPerp(diff[2], diff[1]) * invDet;
            bary[1] = DotPerp(diff[0], diff[2]) * invDet;
            bary[2] = (T)1 - bary[0] - bary[1];
            return true;
        }

        for (int i = 0; i < 3; ++i) {
            bary[i] = (T)0;
        }
        return false;
    }
}