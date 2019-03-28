#pragma once

#include "Math/Vector.h"
#include "Math/Swizzle.h"

namespace NTGS {
    template<typename T>
    struct Matrix<T, 1, 4> : public MatrixOperators<Matrix, T, 1, 4> {
        using Element = T;
        using Index = typename Array<T, 1, 4>::Index;
        static constexpr int Rows = 4;
        static constexpr int Cols = 1;
        static constexpr int Dimension = 4;

        union {
            Array<T, 1, 4> mArray;
            struct { T x, y, z, w; };
            struct { T r, g, b, a; };
            Swizzle<Vector, T, 0, 1, 2, 3> xyzw;
            Swizzle<Vector, T, 3, 0, 1, 2> wxyz;
            struct { Vector<T, 3> xyz; T w; };
            struct { Vector<T, 2> xy; Vector<T, 2> zw; };
        };

        Matrix() {
            std::memset(mArray.mData, 0, sizeof(T) * 4);
        }
        explicit Matrix(const T& num) {
            for (int i = 0; i < 4; i++)
                mArray[i] = num;
        }
        Matrix(const std::initializer_list<T>& list) {
            auto iter = list.begin();
            for (int j = 0; j < 4; j++, iter++)
                mArray[j] = *iter;
        }
        template<typename U, int M, typename = typename std::enable_if<(M >= 4)>::type>
        Matrix(const Vector<U, M>& vec) {
            for (int i = 0; i < 4; i++)
                mArray[i] = static_cast<T>(vec[i]);
        }
        Matrix(const T& a, const T& b, const T& c, const T& d)
            : x(a), y(b), z(c), w(d)
        {}
        Matrix(const Vector<T, 2>& vec, const T& c, const T& d)
            : x(vec[0]), y(vec[1]), z(c), w(d)
        {}
        Matrix(const T& a, const T& b, const Vector<T, 2>& vec)
            : x(a), y(b), z(vec[0]), w(vec[1])
        {}
        Matrix(const Vector<T, 3>& vec, const T& d)
            : x(vec[0]), y(vec[1]), z(vec[2]), w(d)
        {}
        Matrix(const T& a, const Vector<T, 3>& vec)
            : x(a), y(vec[0]), z(vec[1]), w(vec[2])
        {}

        inline T& operator[](int i) {
            assert((uint32_t)i < 4);
            return mArray[i];
        }
        inline const T& operator[](int i) const {
            assert((uint32_t)i < 4);
            return mArray[i];
        }
    };

    typedef Matrix<int, 1, 4> Vec4i;
    typedef Matrix<float, 1, 4> Vec4f;
    typedef Matrix<double, 1, 4> Vec4d;
}