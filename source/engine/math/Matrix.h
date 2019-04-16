#pragma once

#include "math/MatrixOperators.h"
#include "math/Array.h"

namespace NTGS {
    template<typename, int, int>
    struct Matrix;

    template<typename T, int N>
    using Vector = Matrix<T, 1, N>;

    template<typename T, int _Cols, int _Rows>
    struct Matrix : public MatrixOperators<Matrix, T, _Cols, _Rows> {
        using Element = T;
        using Index = typename Array<T, _Cols, _Rows>::Index;
        static constexpr int Rows = _Rows;
        static constexpr int Cols = _Cols;

        Array<T, Cols, Rows> mArray;

        Matrix() { 
            for (int i = 0; i < Cols; i++)
                for (int j = 0; j < Rows; j++)
                    mArray[Index(i, j)] = (i == j ? 1 : 0);
        }
        explicit Matrix(const Vector<T, Rows>& col) {
            for (int i = 0; i < Cols; i++)
                for (int j = 0; j < Rows; j++)
                    mArray[Index(i, j)] = col[j];
        }
        Matrix(const std::initializer_list<Vector<T, Rows>>& list) {
            auto iter = list.begin();
            for (int i = 0; i < Cols; i++, iter++)
                for (int j = 0; j < Rows; j++)
                    mArray[Index(i, j)] = static_cast<T>((*iter)[j]);
        }
        template<typename U, int U_Cols, int U_Rows,
                 typename = typename std::enable_if<(U_Rows >= Rows && U_Cols >= Cols)>::type>
        Matrix(const Matrix<U, U_Cols, U_Rows>& mat) {
            for (int i = 0; i < Cols; i++)
                for (int j = 0; j < Rows; j++)
                    mArray[Index(i, j)] = static_cast<T>(mat.mArray[Index(i, j)]);
        }

        inline Vector<T, Rows>& operator[](int i) {
            assert((uint32_t)i < Cols);
            return *(reinterpret_cast<Vector<T, Rows>*>(&mArray[Index(i, 0)]));
        }

        inline const Vector<T, Rows>& operator[](int i) const {
            assert((uint32_t) i < Cols);
            return *(reinterpret_cast<const Vector<T, Rows>*>(&mArray[Index(i, 0)]));
        }
    };
}
