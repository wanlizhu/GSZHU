#pragma once

#include "Engine/Common/Math.h"
#include "Engine/Geometry/Algebra/Orthonormalize.h"
#include "Engine/Geometry/Algebra/HyperCross.h"
#include "Engine/Geometry/Algebra/Perpendicular.h"

namespace ZHU
{
    /* Compute a right-handed orthonormal basis for the orthogonal complement
     * of the input vectors.  The function returns the smallest length of the
     * unnormalized vectors computed during the process.  If this value is nearly
     * zero, it is possible that the inputs are linearly dependent (within
     * numerical round-off errors).  
     * On input, numInputs must be 1, 2...N, and v[0] through v[numInputs-1] must be initialized.
     * On output, the vectors v[0] through v[N] form an orthonormal set.
     */
    template<typename T, int N>
    class OrthogonalComplement
    {
    public:
        T operator()(int numInputs, Vector<T, N>* basis) const;
    };










    /*
                        TEMPLATE CLASS IMPLEMENTATION
      -------------------------------+----------------------------------
                               CLASS : OrthogonalComplement
                           ARGUMENTS : <typename T, int N>
                         GENERAL IMP : NO
              PARTIAL SPECIALIZATION : YES
                 FULL SPECIALIZATION : NO
      -------------------------------+----------------------------------
    */
    
    template<typename T>
    class OrthogonalComplement<T, 2> 
    {
    public:
        T operator()(int numInputs, Vector<T, 2>* basis) const
        {
            if (numInputs == 1) {
                basis[1] = -Perpendicular<T>()(basis[0]);
                return Orthonormalize<T, 2>()(2, basis);
            }

            return (T)0;
        }
    }; 


    template <typename T>
    class OrthogonalComplement<T, 3>
    {
    public:
        T operator()(int numInputs, Vector<T, 3>* basis) const
        {
            if (numInputs == 1) {
                if (std::abs(basis[0][0]) > std::abs(basis[0][1])) {
                    basis[1] = { -basis[0][2], (T)0, +basis[0][0] };
                }
                else {
                    basis[1] = { (T)0, +basis[0][2], -basis[0][1] };
                }
                numInputs = 2;
            }

            if (numInputs == 2) {
                basis[2] = basis[0].cross(basis[1]);
                return Orthonormalize<T, 3>()(3, basis);
            }

            return (T)0;
        }
    };


    template <typename T>
    class OrthogonalComplement<T, 4>
    {
    public:
        T operator()(int numInputs, Vector<T, 4>* basis) const
        {
            if (numInputs == 1) {
                int maxIndex = 0;
                T maxAbsValue = std::abs(basis[0][0]);
                for (int i = 1; i < 4; ++i) {
                    T absValue = std::abs(basis[0][i]);
                    if (absValue > maxAbsValue) {
                        maxIndex = i;
                        maxAbsValue = absValue;
                    }
                }

                if (maxIndex < 2) {
                    basis[1][0] = -basis[0][1];
                    basis[1][1] = +basis[0][0];
                    basis[1][2] = (T)0;
                    basis[1][3] = (T)0;
                }
                else if (maxIndex == 3) {
                    // Generally, you can skip this clause and swap the last two
                    // components.  However, by swapping 2 and 3 in this case, we
                    // allow the function to work properly when the inputs are 3D
                    // vectors represented as 4D affine vectors (w = 0).
                    basis[1][0] = (T)0;
                    basis[1][1] = +basis[0][2];
                    basis[1][2] = -basis[0][1];
                    basis[1][3] = (T)0;
                }
                else {
                    basis[1][0] = (T)0;
                    basis[1][1] = (T)0;
                    basis[1][2] = -basis[0][3];
                    basis[1][3] = +basis[0][2];
                }

                numInputs = 2;
            }

            if (numInputs == 2) {
                T det[6] =
                {
                    basis[0][0] * basis[1][1] - basis[1][0] * basis[0][1],
                    basis[0][0] * basis[1][2] - basis[1][0] * basis[0][2],
                    basis[0][0] * basis[1][3] - basis[1][0] * basis[0][3],
                    basis[0][1] * basis[1][2] - basis[1][1] * basis[0][2],
                    basis[0][1] * basis[1][3] - basis[1][1] * basis[0][3],
                    basis[0][2] * basis[1][3] - basis[1][2] * basis[0][3]
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
                    basis[2] = { -det[4], +det[2], (T)0, -det[0] };
                }
                else if (maxIndex <= 2) {
                    basis[2] = { +det[5], (T)0, -det[2], +det[1] };
                }
                else {
                    basis[2] = { (T)0, -det[5], +det[4], -det[3] };
                }

                numInputs = 3;
            }

            if (numInputs == 3) {
                basis[3] = HyperCross(basis[0], basis[1], basis[2]);
                return Orthonormalize<T, 4>(4, basis);
            }

            return (T)0;
        }
    };
}