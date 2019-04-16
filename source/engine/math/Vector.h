#pragma once

#include "math/Matrix.h"

namespace NTGS {
    
    template<typename T, int N>
    struct Matrix<T, 1, N> : public MatrixOperators<Matrix, T, 1, N> {
        using Element = T;
        using Index = typename Array<T, 1, N>::Index;
        static constexpr int Rows = N;
        static constexpr int Cols = 1;
        static constexpr int Dimension = N;

        Array<T, 1, N> mArray;

        Matrix() { 
            std::memset(mArray.mData, 0, sizeof(T) * N);
        }
        explicit Matrix(const T& num) {
            for (int i = 0; i < N; i++)
                mArray[i] = num;
        }
        Matrix(const std::initializer_list<T>& list) { 
            auto iter = list.begin();
            for (int j = 0; j < N; j++, iter++)
                mArray[j] = *iter;
        }
        template<typename U, int M, typename = typename std::enable_if<(M >= N)>::type>
        Matrix(const Vector<U, M>& vec) {
            for (int i = 0; i < N; i++)
                mArray[i] = static_cast<T>(vec[i]);
        }

        inline T& operator[](int i) {
            assert((uint32_t) i < N); 
            return mArray[i];
        }
        inline const T& operator[](int i) const {
            assert((uint32_t) i < N); 
            return mArray[i];
        }
    };
}
