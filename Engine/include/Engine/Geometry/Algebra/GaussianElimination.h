#pragma once

#include <cmath>
#include <string>
#include <vector>
#include "./ArrayView.h"

namespace ZHU
{
    /* The input matrix M must be NxN, which is in column major by default.
     * If you want the inverse of M, pass a valid pointer inverseM, otherwise a nullptr.
     * Matrix 'inverseM' must also be NxN and use the same storage convention as M.  
     *
     * If you want to solve M*X = B for X, where X and B are Nx1, pass valid pointers for B and X.  
     * If you want to solve M*Y = C for Y, where X and C are NxK, pass valid pointers for C and Y and pass K to numCols. 
     * In all cases, pass N to numRows.
     */
    template<typename T>
    class GaussianElimination
    {
    public:
        bool operator()(int rows, 
                        const T* M, T* invM, T& detM,
                        const T* B, T* X,
                        const T* C, int cols, T* Y) const;
    private:
        /* This function hides the type traits used to determine 
         * whether T is native floating-point or otherwise (such
         * as BSNumber or BSRational).
         */
        void Set(int num, const T* source, T* target) const;
    };
}