#pragma once

#include "Engine/Common/Math.h"

namespace ZHU
{
    template<typename T, int N>
    class Orthonormalize
    {
    public:
        T operator()(int numInputs, Vector<T, N>* basis) const
        {
            if (basis && 1 <= numInputs && numInputs <= N) {
                T minLength = basis[0].norm();
                basis[0].normalize();
                for (int i = 1; i < numInputs; ++i) {
                    for (int j = 0; j < i; ++j) {
                        T dot = basis[i].dot(basis[j]);
                        basis[i] -= basis[j] * dot;
                    }

                    T length = basis[i].norm(); 
                    basis[i].normalize();
                    if (length < minLength) {
                        minLength = length;
                    }
                }
                return minLength;
            }

            return (T)0;
        }
    };
}