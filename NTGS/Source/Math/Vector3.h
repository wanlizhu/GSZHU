#pragma once

#include "Math/Vector.h"
#include "Math/Swizzle.h"

namespace NTGS {
    template<typename T>
    struct Matrix<T, 1, 3> : public MatrixOperators<Matrix, T, 1, 3> {
        using Element = T;
        using Index = typename Array<T, 1, 3>::Index;
        static constexpr int Rows = 3;
        static constexpr int Cols = 1;
        static constexpr int Dimension = 3;

        union {
            Array<T, 1, 3> mArray;
            struct { T x, y, z; };
            struct { T r, g, b; };
            Swizzle<Vector, T, 0, 1, 2> xyz;
            Swizzle<Vector, T, 0, 2, 1> xzy;
            Swizzle<Vector, T, 2, 1, 0> zyx;
            Swizzle<Vector, T, 2, 0, 1> zxy;
            Swizzle<Vector, T, 1, 0, 2> yxz;
            Swizzle<Vector, T, 1, 2, 0> yzx;
        };

        Matrix() {
            std::memset(mArray.mData, 0, sizeof(T) * 3);
        }
        explicit Matrix(const T& num) {
            for (int i = 0; i < 3; i++)
                mArray[i] = num;
        }
        Matrix(const std::initializer_list<T>& list) {
            auto iter = list.begin();
            for (int j = 0; j < 3; j++, iter++)
                mArray[j] = *iter;
        }
        template<typename U, int M, typename = typename std::enable_if<(M >= 3)>::type>
        Matrix(const Vector<U, M>& vec) {
            for (int i = 0; i < 3; i++)
                mArray[i] = static_cast<T>(vec[i]);
        }
        Matrix(const T& a, const T& b, const T& c)
            : x(a), y(b), z(c)
        {}
        Matrix(const Vector<T, 2>& vec, const T& c)
            : x(vec[0]), y(vec[1]), z(c)
        {}
        Matrix(const T& a, const Vector<T, 2>& vec)
            : x(a), y(vec[0]), z(vec[1])
        {}

        inline T& operator[](int i) {
            assert((uint32_t)i < 3);
            return mArray[i];
        }
        inline const T& operator[](int i) const {
            assert((uint32_t)i < 3);
            return mArray[i];
        }
    };

    typedef Matrix<int, 1, 3> Vec3i;
    typedef Matrix<float, 1, 3> Vec3f;
    typedef Matrix<double, 1, 3> Vec3d;
}