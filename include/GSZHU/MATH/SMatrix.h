#pragma once

#include "SVector.h"

namespace GSZHU {
    template<typename T, int M, int N>
    struct SMatrix : public SVector<SVector<T, M>, N> {
        using BASE = SVector<SVector<T, M>, N>;
        using ELEMENT = T;
        using COLUMN = SVector<T, M>;
        using ROW = SVector<T, N>;
        static constexpr int LENGTH = M * N;
        static constexpr int ROWS = M;
        static constexpr int COLUMNS = N;

        SMatrix() noexcept : BASE() {}
        explicit SMatrix(const COLUMN& Col) noexcept : BASE(Col) {}
        SMatrix(const COLUMN& Col0, const COLUMN& Col1, const COLUMN& Col2 = {}, const COLUMN& Col3 = {}) noexcept {
            BASE::operator[](0) = Col0;
            BASE::operator[](1) = Col1;
            if (COLUMNS >= 3) BASE::operator[](2) = Col2;
            if (COLUMNS >= 4) BASE::operator[](3) = Col3;
        }

        template<int M2, int N2, typename = std::enable_if_t<M2 >= M && N2 >= N>>
        SMatrix(const SMatrix<T, M2, N2>& M) noexcept {
            for (int i = 0; i < N; i++)
                BASE::operator[](i) = M[i];
        }

        inline COLUMN* GetData() noexcept { return (COLUMN*)this; }
        inline const COLUMN* GetData() const noexcept { return (const COLUMN*)this; }
        inline void SetData(const T* Src) {
            for (int c = 0; c < COLUMNS; c++)
                for (int r = 0; r < ROWS; r++)
                    BASE::operator[](c)[r] = Src[COLUMNS * c + r];
        }

        inline ROW GetRow(int i) const noexcept { 
            assert(i>= 0 && i <= M);
            ROW Row;
            for (int j = 0; j < N; j++)
                Row[j] = BASE::operator[](j)[i];
            return Row;
        }

        inline void SetRow(int i, const T* Row) noexcept {
            assert(i >= 0 && i <= M);
            for (int j = 0; j < N; j++)
                BASE::operator[](j)[i] = Row[j];
        }

        inline SMatrix operator*=(const ELEMENT& Val) noexcept {
            for (int i = 0; i < COLUMNS; i++)
                BASE::operator[](i) *= Val;
            return *this;
        }

        inline SMatrix operator/=(const ELEMENT& Val) noexcept {
            for (int i = 0; i < COLUMNS; i++)
                BASE::operator[](i) /= Val;
            return *this;
        }
    };
}