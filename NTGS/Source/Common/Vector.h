#pragma once

#include "Common/Matrix.h"

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
        template<typename U, int M, typename = std::enable_if<(M >= N)>::type>
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
        template<typename U, int M, typename = std::enable_if<(M >= 2)>::type>
        Matrix(const Vector<U, M>& vec) {
            for (int i = 0; i < 2; i++)
                mArray[i] = static_cast<T>(vec[i]);
        }
        Matrix(const T& a, const T& b) 
            : x(a), y(b)
        {}

        inline T& operator[](int i) {
            assert((uint32_t) i < 2);
            return mArray[i];
        }
        inline const T& operator[](int i) const {
            assert((uint32_t) i < 2);
            return mArray[i];
        }
    };

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
        template<typename U, int M, typename = std::enable_if<(M >= 3)>::type>
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
            assert((uint32_t) i < 3);
            return mArray[i];
        }
        inline const T& operator[](int i) const {
            assert((uint32_t) i < 3);
            return mArray[i];
        }
    };

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
        template<typename U, int M, typename = std::enable_if<(M >= 4)>::type>
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
            assert((uint32_t) i < 4); 
            return mArray[i];
        }
        inline const T& operator[](int i) const { 
            assert((uint32_t) i < 4); 
            return mArray[i];
        }
    };

    typedef Matrix<int, 1, 2> Vec2i;
    typedef Matrix<float, 1, 2> Vec2f;
    typedef Matrix<double, 1, 2> Vec2d;
    typedef Matrix<int, 1, 3> Vec3i;
    typedef Matrix<float, 1, 3> Vec3f;
    typedef Matrix<double, 1, 3> Vec3d;
    typedef Matrix<int, 1, 4> Vec4i;
    typedef Matrix<float, 1, 4> Vec4f;
    typedef Matrix<double, 1, 4> Vec4d;
}