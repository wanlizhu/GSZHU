#pragma once

#include "Math/Matrix.h"
#include "Math/Matrix4.h"

namespace NTGS {
    template<typename T, int Cols, int Rows>
    inline Vector<T, Rows> GetColumn(const Matrix<T, Cols, Rows>& m, int col) {
        using Index = typename Matrix<T, Cols, Rows>::Index;
        Vector<T, Rows> column;
        for (int i = 0; i < Rows; i++)
            column[i] = m.mArray[Index(col, i)];
        return column;
    }

    template<typename T, int Cols, int Rows>
    inline Vector<T, Cols> GetRow(const Matrix<T, Cols, Rows>& m, int rowId) {
        using Index = typename Matrix<T, Cols, Rows>::Index;
        Vector<T, Cols> row;
        for (int i = 0; i < Cols; i++)
            row[i] = m.mArray[Index(i, rowId)];
        return row;
    }


    template<typename T, int LCols, int LRows,
        typename U, int RCols, int RRows,
        typename = typename std::enable_if<LCols == RRows>::type>
        inline Matrix<T, RCols, LRows> operator*(const Matrix<T, LCols, LRows>& m1,
        const Matrix<U, RCols, RRows>& m2) {
        using Index = typename Matrix<T, RCols, LRows>::Index;
        Matrix<T, RCols, LRows> result;
        for (int i = 0; i < RCols; i++)
            for (int j = 0; j < LRows; j++)
                result.mArray[Index(i, j)] = GetRow(m1, j) * GetColumn(m2, i);
        return result;
    }

    template<typename T, int MN>
    inline Matrix<T, MN, MN> Inverse(const Matrix<T, MN, MN>& m) {

    }

    template<typename T>
    inline Matrix<T, 4, 4> Inverse(const Matrix<T, 4, 4>& m) {
        T coef00 = m[2][2] * m[3][3] - m[3][2] * m[2][3];
        T coef02 = m[1][2] * m[3][3] - m[3][2] * m[1][3];
        T coef03 = m[1][2] * m[2][3] - m[2][2] * m[1][3];

        T coef04 = m[2][1] * m[3][3] - m[3][1] * m[2][3];
        T coef06 = m[1][1] * m[3][3] - m[3][1] * m[1][3];
        T coef07 = m[1][1] * m[2][3] - m[2][1] * m[1][3];

        T coef08 = m[2][1] * m[3][2] - m[3][1] * m[2][2];
        T coef10 = m[1][1] * m[3][2] - m[3][1] * m[1][2];
        T coef11 = m[1][1] * m[2][2] - m[2][1] * m[1][2];

        T coef12 = m[2][0] * m[3][3] - m[3][0] * m[2][3];
        T coef14 = m[1][0] * m[3][3] - m[3][0] * m[1][3];
        T coef15 = m[1][0] * m[2][3] - m[2][0] * m[1][3];

        T coef16 = m[2][0] * m[3][2] - m[3][0] * m[2][2];
        T coef18 = m[1][0] * m[3][2] - m[3][0] * m[1][2];
        T coef19 = m[1][0] * m[2][2] - m[2][0] * m[1][2];

        T coef20 = m[2][0] * m[3][1] - m[3][0] * m[2][1];
        T coef22 = m[1][0] * m[3][1] - m[3][0] * m[1][1];
        T coef23 = m[1][0] * m[2][1] - m[2][0] * m[1][1];

        Vector<T, 4> fac0(coef00, coef00, coef02, coef03);
        Vector<T, 4> fac1(coef04, coef04, coef06, coef07);
        Vector<T, 4> fac2(coef08, coef08, coef10, coef11);
        Vector<T, 4> fac3(coef12, coef12, coef14, coef15);
        Vector<T, 4> fac4(coef16, coef16, coef18, coef19);
        Vector<T, 4> fac5(coef20, coef20, coef22, coef23);

        Vector<T, 4> vec0(m[1][0], m[0][0], m[0][0], m[0][0]);
        Vector<T, 4> vec1(m[1][1], m[0][1], m[0][1], m[0][1]);
        Vector<T, 4> vec2(m[1][2], m[0][2], m[0][2], m[0][2]);
        Vector<T, 4> vec3(m[1][3], m[0][3], m[0][3], m[0][3]);

        Vector<T, 4> inv0(Mul(vec1, fac0) - Mul(vec2, fac1) + Mul(vec3, fac2));
        Vector<T, 4> inv1(Mul(vec0, fac0) - Mul(vec2, fac3) + Mul(vec3, fac4));
        Vector<T, 4> inv2(Mul(vec0, fac1) - Mul(vec1, fac3) + Mul(vec3, fac5));
        Vector<T, 4> inv3(Mul(vec0, fac2) - Mul(vec1, fac4) + Mul(vec2, fac5));

        Vector<T, 4> signA(+1, -1, +1, -1);
        Vector<T, 4> signB(-1, +1, -1, +1);
        Matrix<T, 4, 4> invMat(Mul(inv0, signA), Mul(inv1, signB), Mul(inv2, signA), Mul(inv3, signB));

        Vector<T, 4> row0(invMat[0][0], invMat[1][0], invMat[2][0], invMat[3][0]);

        Vector<T, 4> dot0(Mul(m[0], row0));
        T dot1 = (dot0.x + dot0.y) + (dot0.z + dot0.w);

        T rcpDet = static_cast<T>(1) / dot1;

        return invMat * rcpDet;
    }

    template<typename T, int MN>
    inline T Determinant(const Matrix<T, MN, MN>& m) {

    }

    template<typename T>
    inline T Determinant(const Matrix<T, 4, 4>& m) {
        T fac00 = m[2][2] * m[3][3] - m[3][2] * m[2][3];
        T fac01 = m[2][1] * m[3][3] - m[3][1] * m[2][3];
        T fac02 = m[2][1] * m[3][2] - m[3][1] * m[2][2];
        T fac03 = m[2][0] * m[3][3] - m[3][0] * m[2][3];
        T fac04 = m[2][0] * m[3][2] - m[3][0] * m[2][2];
        T fac05 = m[2][0] * m[3][1] - m[3][0] * m[2][1];

        Vector<T, 4> coef(
            +(m[1][1] * fac00 - m[1][2] * fac01 + m[1][3] * fac02),
            -(m[1][0] * fac00 - m[1][2] * fac03 + m[1][3] * fac04),
            +(m[1][0] * fac01 - m[1][1] * fac03 + m[1][3] * fac05),
            -(m[1][0] * fac02 - m[1][1] * fac04 + m[1][2] * fac05));

        return m[0][0] * coef[0] + m[0][1] * coef[1] +
            m[0][2] * coef[2] + m[0][3] * coef[3];
    }

    template<typename T, int Cols, int Rows>
    inline Matrix<T, Rows, Cols> Transpose(const Matrix<T, Cols, Rows>& m) {
        using OGIndex = typename Matrix<T, Cols, Rows>::Index;
        using TSIndex = typename Matrix<T, Rows, Cols>::Index;
        Matrix<T, Rows, Cols> result;
        for (int i = 0; i < Cols; i++)
            for (int j = 0; j < Rows; j++)
                result.mArray[TSIndex(j, i)] = m.mArray[OGIndex(i, j)];
        return result;
    }

    template<typename BlockType, typename T, int Cols, int Rows,
        typename = typename std::enable_if<BlockType::Rows <= Rows
        && BlockType::Cols <= Cols>::type>
        inline BlockType CopyBlock(const Matrix<T, Cols, Rows>& mat, int fromCol = 0, int fromRow = 0) {
        assert((fromCol + BlockType::Cols <= Cols)
            && (fromRow + BlockType::Rows <= Rows));
        using Index = typename Matrix<T, Cols, Rows>::Index;
        BlockType block;
        for (int i = 0; i < BlockType::Cols; i++)
            for (int j = 0; j < BlockType::Rows; j++)
                block.mArray[typename BlockType::Index(i, j)] = mat.mArray[Index(fromCol + i, fromRow + j)];
        return block;
    }
}