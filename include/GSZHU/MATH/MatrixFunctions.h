#pragma once

#include "SMatrix.h"

namespace GSZHU {
    template<typename T, int M, int N>
    inline SMatrix<T, N, M> Transpose(const SMatrix<T, M, N>& Mat) noexcept {
        SMatrix<T, N, M> Ret;
        for (int i = 0; i < N; i++)
            for (int j = 0; j < M; j++)
                Ret[j][i] = Mat[i][j];
        return Ret;
    }

    template<typename T, int ROWS1, int SHARED, int COLS2>
    inline SMatrix<T, ROWS1, COLS2> operator*(const SMatrix<T, ROWS1, SHARED>& M1, const SMatrix<T, SHARED, COLS2>& M2) noexcept {
        using RIGHT_COLUMN = typename SMatrix<T, SHARED, COLS2>::COLUMN;
        using LEFT_ROW = typename SMatrix<T, ROWS1, SHARED>::ROW;
        SMatrix<T, ROWS1, COLS2> Ret;

        for (int i = 0; i < COLS2; i++) {
            for (int j = 0; j < ROWS1; j++) {
                const RIGHT_COLUMN& Column = M2[i];
                const LEFT_ROW& Row = M1.GetRow(j);
                Ret[i][j] = Dot(Row, Column);
            }
        }

        return Ret;
    }

    template<typename T, int ROWS, int COLS>
    inline SVector<T, ROWS> operator*(const SMatrix<T, ROWS, COLS>& M, const SVector<T, ROWS>& V) noexcept {
        SMatrix<T, ROWS, 1> Ret = M * *reinterpret_cast<const SMatrix<T, ROWS, 1>*>(&V);
        return *reinterpret_cast<const SVector<T, ROWS>*>(&Ret);
    }

    template<typename T, int N>
    inline SMatrix<T, N, N> operator~(const SMatrix<T, N, N>& M) noexcept {
        SMatrix<T, N, N> Ret;
        static_assert(false); // TODO
        return Ret;
    }

    template<typename T>
    inline SMatrix<T, 4, 4> operator~(const SMatrix<T, 4, 4>& M) noexcept {
        T coef00 = M[2][2] * M[3][3] - M[3][2] * M[2][3];
        T coef02 = M[1][2] * M[3][3] - M[3][2] * M[1][3];
        T coef03 = M[1][2] * M[2][3] - M[2][2] * M[1][3];

        T coef04 = M[2][1] * M[3][3] - M[3][1] * M[2][3];
        T coef06 = M[1][1] * M[3][3] - M[3][1] * M[1][3];
        T coef07 = M[1][1] * M[2][3] - M[2][1] * M[1][3];

        T coef08 = M[2][1] * M[3][2] - M[3][1] * M[2][2];
        T coef10 = M[1][1] * M[3][2] - M[3][1] * M[1][2];
        T coef11 = M[1][1] * M[2][2] - M[2][1] * M[1][2];

        T coef12 = M[2][0] * M[3][3] - M[3][0] * M[2][3];
        T coef14 = M[1][0] * M[3][3] - M[3][0] * M[1][3];
        T coef15 = M[1][0] * M[2][3] - M[2][0] * M[1][3];

        T coef16 = M[2][0] * M[3][2] - M[3][0] * M[2][2];
        T coef18 = M[1][0] * M[3][2] - M[3][0] * M[1][2];
        T coef19 = M[1][0] * M[2][2] - M[2][0] * M[1][2];

        T coef20 = M[2][0] * M[3][1] - M[3][0] * M[2][1];
        T coef22 = M[1][0] * M[3][1] - M[3][0] * M[1][1];
        T coef23 = M[1][0] * M[2][1] - M[2][0] * M[1][1];

        SVector<T, 4> fac0(coef00, coef00, coef02, coef03);
        SVector<T, 4> fac1(coef04, coef04, coef06, coef07);
        SVector<T, 4> fac2(coef08, coef08, coef10, coef11);
        SVector<T, 4> fac3(coef12, coef12, coef14, coef15);
        SVector<T, 4> fac4(coef16, coef16, coef18, coef19);
        SVector<T, 4> fac5(coef20, coef20, coef22, coef23);

        SVector<T, 4> vec0(M[1][0], M[0][0], M[0][0], M[0][0]);
        SVector<T, 4> vec1(M[1][1], M[0][1], M[0][1], M[0][1]);
        SVector<T, 4> vec2(M[1][2], M[0][2], M[0][2], M[0][2]);
        SVector<T, 4> vec3(M[1][3], M[0][3], M[0][3], M[0][3]);

        SVector<T, 4> inv0((vec1 * fac0) - (vec2 * fac1) + (vec3 * fac2));
        SVector<T, 4> inv1((vec0 * fac0) - (vec2 * fac3) + (vec3 * fac4));
        SVector<T, 4> inv2((vec0 * fac1) - (vec1 * fac3) + (vec3 * fac5));
        SVector<T, 4> inv3((vec0 * fac2) - (vec1 * fac4) + (vec2 * fac5));

        SVector<T, 4> signA(+1, -1, +1, -1);
        SVector<T, 4> signB(-1, +1, -1, +1);
        SMatrix<T, 4, 4> invMat((inv0 * signA), (inv1 * signB), (inv2 * signA), (inv3 * signB));

        SVector<T, 4> row0(invMat[0][0], invMat[1][0], invMat[2][0], invMat[3][0]);

        SVector<T, 4> dot0((M[0] * row0));
        T dot1 = (dot0.GetX() + dot0.GetY()) + (dot0.GetZ() + dot0.GetW());

        T rcpDet = (T)(1) / dot1;

        return invMat * rcpDet;
    }


    // 2 * A
    template<typename T, int M, int N>
    inline SMatrix<T, M, N> operator*(const T& Val, const SMatrix<T, M, N>& M2) {
        return M2 * Val;
    }

    // A * 2
    template<typename T, int M, int N>
    inline SMatrix<T, M, N> operator*(const SMatrix<T, M, N>& M1, const T& Val) {
        return SMatrix<T, M, N>(M1) *= Val;
    }

    // A / 2
    template<typename T, int M, int N>
    inline SMatrix<T, M, N> operator/(const SMatrix<T, M, N>& M1, const T& Val) {
        return SMatrix<T, M, N>(M1) /= Val;
    }
}