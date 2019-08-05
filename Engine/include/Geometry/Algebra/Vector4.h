#pragma once

#include "Geometry/Algebra/Vector.h"

namespace ZHU
{
    template<typename T, bool CM = true>
    using Vector4 = Vector<T, 4, CM>;


    /* In GteVector3.h, the Vector3 Cross, UnitCross, and DotCross have a template
     * parameter N that should be 3 or 4.  The latter case supports affine vectors
     * in 4D (last component w = 0) when you want to use 4-tuples and 4x4 matrices
     * for affine algebra.  Thus, you may use those template functions for Vector4.
     */

    /* Compute the hypercross product using the formal determinant:
     *   hcross = det{{e0,e1,e2,e3},{x0,x1,x2,x3},{y0,y1,y2,y3},{z0,z1,z2,z3}}
     * where e0 = (1,0,0,0), e1 = (0,1,0,0), e2 = (0,0,1,0), e3 = (0,0,0,1),
     * v0 = (x0,x1,x2,x3), v1 = (y0,y1,y2,y3), and v2 = (z0,z1,z2,z3).
     */
    template<typename T, bool CM = true>
    Vector4<T, CM> HyperCross(const Vector4<T, CM>& v0,
                              const Vector4<T, CM>& v1,
                              const Vector4<T, CM>& v2);


    // Compute the normalized hypercross product.
    template <typename T, bool CM = true>
    Vector4<T, CM> UnitHyperCross(const Vector4<T, CM>& v0,
                                  const Vector4<T, CM>& v1,
                                  const Vector4<T, CM>& v2,
                                  bool robust = false);


    /* Compute Dot(HyperCross((x0,x1,x2,x3),(y0,y1,y2,y3),(z0,z1,z2,z3)),
     * (w0,w1,w2,w3)), where v0 = (x0,x1,x2,x3), v1 = (y0,y1,y2,y3),
     * v2 = (z0,z1,z2,z3), and v3 = (w0,w1,w2,w3).
     *
     * = Dot(HyperCross(v0, v1, v2), v3);
     */
    template <typename T, bool CM = true>
    T DotHyperCross(const Vector4<T, CM>& v0,
                    const Vector4<T, CM>& v1,
                    const Vector4<T, CM>& v2,
                    const Vector4<T, CM>& v3);


    /* Compute a right-handed orthonormal basis for the orthogonal complement
     * of the input vectors.  The function returns the smallest length of the
     * unnormalized vectors computed during the process.  If this value is nearly
     * zero, it is possible that the inputs are linearly dependent (within
     * numerical round-off errors).  On input, numInputs must be 1, 2, or 3 and
     * v[0] through v[numInputs-1] must be initialized.  On output, the vectors
     * v[0] through v[3] form an orthonormal set.
     */
    template<typename T, bool CM = true>
    T ComputeOrthoComplement(int numInputs, Vector4<T, CM>* v,
                             bool robust = false);















    /*
                        TEMPLATE CLASS IMPLEMENTATION
      -------------------------------+----------------------------------
                            FUNCTION : for Vector4<T>
                           ARGUMENTS : <typename T>
                         GENERAL IMP : YES
              PARTIAL SPECIALIZATION : NO
                 FULL SPECIALIZATION : NO
      -------------------------------+----------------------------------
    */

    template<typename T, bool CM = true>
    Vector4<T> HyperCross(const Vector4<T>& v0, const Vector4<T>& v1, const Vector4<T>& v2)
    {
        T m01 = v0[0] * v1[1] - v0[1] * v1[0];  // x0*y1 - y0*x1
        T m02 = v0[0] * v1[2] - v0[2] * v1[0];  // x0*z1 - z0*x1
        T m03 = v0[0] * v1[3] - v0[3] * v1[0];  // x0*w1 - w0*x1
        T m12 = v0[1] * v1[2] - v0[2] * v1[1];  // y0*z1 - z0*y1
        T m13 = v0[1] * v1[3] - v0[3] * v1[1];  // y0*w1 - w0*y1
        T m23 = v0[2] * v1[3] - v0[3] * v1[2];  // z0*w1 - w0*z1
        return Vector4<T>
        {
            +m23 * v2[1] - m13 * v2[2] + m12 * v2[3],  // +m23*y2 - m13*z2 + m12*w2
            -m23 * v2[0] + m03 * v2[2] - m02 * v2[3],  // -m23*x2 + m03*z2 - m02*w2
            +m13 * v2[0] - m03 * v2[1] + m01 * v2[3],  // +m13*x2 - m03*y2 + m01*w2
            -m12 * v2[0] + m02 * v2[1] - m01 * v2[2]   // -m12*x2 + m02*y2 - m01*z2
        };
    }


    template <typename T, bool CM = true>
    Vector4<T> UnitHyperCross(const Vector4<T>& v0, const Vector4<T>& v1, const Vector4<T>& v2,
                              bool robust)
    {
        Vector4<T> unitHyperCross = HyperCross(v0, v1, v2);
        Normalize(unitHyperCross, robust);
        return unitHyperCross;
    }



    template <typename T, bool CM = true>
    T DotHyperCross(const Vector4<T>& v0, const Vector4<T>& v1, const Vector4<T>& v2, 
                    const Vector4<T>& v3)
    {
        return Dot(HyperCross(v0, v1, v2), v3);
    }



    template<typename T, bool CM = true>
    T ComputeOrthoComplement(int numInputs, Vector4<T>* v,
                             bool robust)
    {
        if (numInputs == 1) {
            int maxIndex = 0;
            T maxAbsValue = std::abs(v[0][0]);
            for (int i = 1; i < 4; ++i) {
                T absValue = std::abs(v[0][i]);
                if (absValue > maxAbsValue) {
                    maxIndex = i;
                    maxAbsValue = absValue;
                }
            }

            if (maxIndex < 2) {
                v[1][0] = -v[0][1];
                v[1][1] = +v[0][0];
                v[1][2] = (T)0;
                v[1][3] = (T)0;
            }
            else if (maxIndex == 3) {
                // Generally, you can skip this clause and swap the last two
                // components.  However, by swapping 2 and 3 in this case, we
                // allow the function to work properly when the inputs are 3D
                // vectors represented as 4D affine vectors (w = 0).
                v[1][0] = (T)0;
                v[1][1] = +v[0][2];
                v[1][2] = -v[0][1];
                v[1][3] = (T)0;
            }
            else {
                v[1][0] = (T)0;
                v[1][1] = (T)0;
                v[1][2] = -v[0][3];
                v[1][3] = +v[0][2];
            }

            numInputs = 2;
        }

        if (numInputs == 2) {
            T det[6] =
            {
                v[0][0] * v[1][1] - v[1][0] * v[0][1],
                v[0][0] * v[1][2] - v[1][0] * v[0][2],
                v[0][0] * v[1][3] - v[1][0] * v[0][3],
                v[0][1] * v[1][2] - v[1][1] * v[0][2],
                v[0][1] * v[1][3] - v[1][1] * v[0][3],
                v[0][2] * v[1][3] - v[1][2] * v[0][3]
            };

            int maxIndex = 0;
            T maxAbsValue = std::abs(det[0]);
            for (int i = 1; i < 6; ++i) {
                T absValue = std::abs(det[i]);
                if (absValue > maxAbsValue) {
                    maxIndex = i;
                    maxAbsValue = absValue;
                }
            }

            if (maxIndex == 0) {
                v[2] = { -det[4], +det[2], (T)0, -det[0] };
            }
            else if (maxIndex <= 2) {
                v[2] = { +det[5], (T)0, -det[2], +det[1] };
            }
            else {
                v[2] = { (T)0, -det[5], +det[4], -det[3] };
            }

            numInputs = 3;
        }

        if (numInputs == 3) {
            v[3] = HyperCross(v[0], v[1], v[2]);
            return Orthonormalize<4, T>(4, v, robust);
        }

        return (T)0;
    }
}