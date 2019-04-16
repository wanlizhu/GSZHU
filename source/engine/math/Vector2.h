#pragma once

#include "math/Vector.h"
#include "math/Swizzle.h"

namespace NTGS {
    template<typename T>
    struct Matrix<T, 1, 2> : public MatrixOperators<Matrix, T, 1, 2> {
        using Element = T;
        using Index = typename Array<T, 1, 2>::Index;
        static constexpr int Rows = 2;
        static constexpr int Cols = 1;
        static constexpr int Dimension = 2;

        union {
            Array<T, 1, 2> mArray;
            struct { T x, y; };
            struct { T u, v; };
            Swizzle<Vector, T, 0, 1> xy;
            Swizzle<Vector, T, 1, 0> yx;
        };

        Matrix() {
            std::memset(mArray.mData, 0, sizeof(T) * 2);
        }
        explicit Matrix(const T& num) {
            for (int i = 0; i < 2; i++)
                mArray[i] = num;
        }
        Matrix(const std::initializer_list<T>& list) {
            auto iter = list.begin();
            for (int j = 0; j < 2; j++, iter++)
                mArray[j] = *iter;
        }
        template<typename U, int M, typename = typename std::enable_if<(M >= 2)>::type>
        Matrix(const Vector<U, M>& vec) {
            for (int i = 0; i < 2; i++)
                mArray[i] = static_cast<T>(vec[i]);
        }
        Matrix(const T& a, const T& b)
            : x(a), y(b)
        {}

        inline T& operator[](int i) {
            assert((uint32_t)i < 2);
            return mArray[i];
        }
        inline const T& operator[](int i) const {
            assert((uint32_t)i < 2);
            return mArray[i];
        }
    };

    typedef Matrix<int, 1, 2> Vec2i;
    typedef Matrix<float, 1, 2> Vec2f;
    typedef Matrix<double, 1, 2> Vec2d;
}