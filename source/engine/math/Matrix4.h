#pragma once

#include "math/Matrix.h"

namespace NTGS {
    template<typename T>
    struct Matrix<T, 4, 4> : public MatrixOperators<Matrix, T, 4, 4> {
        using Element = T;
        using Index = typename Array<T, 4, 4>::Index;
        static constexpr int Rows = 4;
        static constexpr int Cols = 4;

        Array<T, 4, 4> mArray;

        Matrix() {
            for (int i = 0; i < 4; i++)
                for (int j = 0; j < 4; j++)
                    mArray[Index(i, j)] = (i == j ? T(1) : T(0));
        }
        explicit Matrix(const Vector<T, 4>& col) {
            for (int i = 0; i < 4; i++)
                for (int j = 0; j < 4; j++)
                    mArray[Index(i, j)] = col[j];
        }
        Matrix(const std::initializer_list<Vector<T, 4>>& list) {
            auto iter = list.begin();
            for (int i = 0; i < 4; i++, iter++)
                for (int j = 0; j < 4; j++)
                    mArray[Index(i, j)] = static_cast<T>((*iter)[j]);
        }
        template<typename U, int U_Cols, int U_Rows,
            typename = typename std::enable_if<(U_Rows >= 4 && U_Cols >= 4)>::type>
            Matrix(const Matrix<U, U_Cols, U_Rows>& mat) {
            for (int i = 0; i < 4; i++)
                for (int j = 0; j < 4; j++)
                    mArray[Index(i, j)] = static_cast<T>(mat.mArray[Index(i, j)]);
        }
        Matrix(const Vector<T, 4>& col0,
            const Vector<T, 4>& col1,
            const Vector<T, 4>& col2,
            const Vector<T, 4>& col3) {
            (*this)[0] = col0;
            (*this)[1] = col1;
            (*this)[2] = col2;
            (*this)[3] = col3;
        }

        inline Vector<T, 4>& operator[](int i) {
            assert((uint32_t)i < 4);
            return *(reinterpret_cast<Vector<T, 4>*>(&mArray[Index(i, 0)]));
        }

        inline const Vector<T, 4>& operator[](int i) const {
            assert((uint32_t)i < 4);
            return *(reinterpret_cast<const Vector<T, 4>*>(&mArray[Index(i, 0)]));
        }

        template<typename U>
        inline Vector<U, 3> operator()(const Vector<U, 3>& vec) const {
            return *this * Vector<U, 4>(vec, U(0));
        }
    };



    typedef Matrix<int, 4, 4> Mat4i;
    typedef Matrix<float, 4, 4> Mat4f;
    typedef Matrix<double, 4, 4> Mat4d;
}