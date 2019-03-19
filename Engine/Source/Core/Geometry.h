#pragma once

#include "BasicTypes.h"

namespace NTGS {
    /////////////////////////////// VECTOR
    template<template<typename T, int N> class VEC, typename T, int N>
    inline T Dot(const VEC<T, N>& v1, const VEC<T, N>& v2) {
        T sum = 0;
        for (int i = 0; i < N; i++) {
            sum += v1[i] * v2[i];
        }
        return sum;
    }

    template<template<typename T, int N> class VEC, typename T>
    inline VEC<T, 3> Cross(const VEC<T, 3>& v1, const VEC<T, 3>& v2) {
        return VEC<T, 3>(
            v1.y * v2.z - v2.y * v1.z,
            v1.z * v2.x - v2.z * v1.x,
            v1.x * v2.y - v2.x * v1.y);
    }

    template<template<typename T, int N> class VEC, typename T, int N>
    inline T Length(const VEC<T, N>& v) {
        T sum = 0;
        for (int i = 0; i < N; i++) {
            sum += v[i] * v[i];
        }
        return std::sqrt(sum);
    }

    template<template<typename T, int N> class VEC, typename T, int N>
    inline bool IsNormalized(const VEC<T, N>& v) {
        return std::abs(Length(v) - T(1)) <= std::numeric_limits<T>::epsilon();
    }

    template<template<typename T, int N> class VEC, typename T, int N>
    inline T Distance(const VEC<T, N>& v1, const VEC<T, N>& v2) {
        return Length(v2 - v1);
    }

    template<template<typename T, int N> class VEC, typename T, int N>
    inline VEC<T, N> Normalize(const VEC<T, N>& v) {
        if (IsNormalized(v))
            return v;
        else{
            VEC<T, N> inst(v);
            T len = Length(v);
            for (int i = 0; i < N; i++) {
                inst[i] /= len;
            }
            return inst;
        }
    }

    template<template<typename T, int N> class VEC, typename T, int N>
    inline VEC<T, N> Reflect(const VEC<T, N>& in, const VEC<T, N>& normal) {
        return in - normal * Dot(normal, in) * T(2);
    }

    template<template<typename T, int N> class VEC, typename T, int N>
    inline VEC<T, N> Refract(const VEC<T, N>& in, const VEC<T, N>& normal, T eta) {
        const T dotValue = Dot(normal, in);
        const T k = T(1) - eta * eta * (T(1) - dotValue * dotValue);
        return (eta * in - (eta * dotValue + std::sqrt(k)) * normal) * T(k >= T(0));
    }

    template<template<typename T, int N> class VEC, typename T, int N>
    inline VEC<T, N> ConvertLinearToSRGB(const VEC<T, N>& colorLinear, T gamma) {

    }

    template<template<typename T, int N> class VEC, typename T, int N>
    inline VEC<T, N> ConvertSRGBToLinear(const VEC<T, N>& colorSRGB, T gamma) {

    }

    template<template<typename T, int N> class VEC, typename T, int N>
    inline VEC<T, N> ClosestPoint(const VEC<T, N>& point, const VEC<T, N>& begin, const VEC<T, N>& end) {
        
    }
      


    /////////////////////////////// MATRIX
    template<template<typename T, int Rows, int Cols> class MAT, typename T, int Rows, int Cols,
             typename = std::enable_if<Rows == Cols>>
    inline void SetIdentity(MAT<T, Rows, Cols>& m) {
        m.Set(typename MAT<T, Rows, Cols>::ColumnType(0));
        for (int i = 0; i < Cols; i++) {
            m[i][i] = T(1);
        }
    }

    template<template<typename T, int Rows, int Cols> class MAT, typename T, int Rows, int Cols>
    inline Vector<T, Rows> Column(const MAT<T, Rows, Cols>& m, int idx) {
        assert(idx >= 0 && idx < Cols);
        Vector<T, Rows> column;
        for (int i = 0; i < Rows; i++) {
            column[i] = m[idx][i];
        }
        return column;
    }

    template<template<typename T, int Rows, int Cols> class MAT, typename T, int Rows, int Cols>
    inline Vector<T, Cols> Row(const MAT<T, Rows, Cols>& m, int idx) {
        assert(idx >= 0 && idx < Rows);
        Vector<T, Cols> row;
        for (int i = 0; i < Cols; i++) {
            row[i] = m[i][idx];
        }
        return row;
    }

    template<template<typename T, int Rows, int Cols> class MAT, typename T, int Rows, int Cols>
    inline bool IsIdentity(const MAT<T, Rows, Cols>& m) {
        if (Rows != Cols)
            return false;
        for (int i = 0; i < Cols; i++) {
            for (int j = 0; j < Rows; j++) {
                if (i == j) {
                    if (std::abs(m[i][j] - T(1)) > std::numeric_limits<T>::epsilon())
                        return false;
                }
                else {
                    if (std::abs(m[i][j] - T(0)) > std::numeric_limits<T>::epsilon())
                        return false;
                }
            }
        }
        return true;
    }

    template<template<typename T, int Rows, int Cols> class MAT, typename T, int Rows, int Cols>
    inline MAT<T, Cols, Rows> Transpose(const MAT<T, Rows, Cols>& m) {
        MAT<T, Cols, Rows> result;
        for (int i = 0; i < Cols; i++) {
            for (int j = 0; j < Rows; j++) {
                result[j][i] = m[i][j];
            }
        }
        return result;
    }

    template<template<typename T, int Rows, int Cols> class MAT, typename T, int Rows, int Cols>
    inline T Determinant(const MAT<T, Rows, Cols>& m) {
        
    }

    template<template<typename T, int Rows, int Cols> class MAT, typename T, int Rows, int Cols,
             typename = std::enable_if<Rows == Cols>>
    inline MAT<T, Rows, Cols> Inverse(const MAT<T, Rows, Cols>& m) {
        
    }

    template<template<typename T, int Rows, int Cols> class MAT, 
             typename T, int L_Rows, int L_Cols,
                         int R_Rows, int R_Cols,
             typename = std::enable_if<L_Cols == R_Rows>>
    inline MAT<T, L_Rows, R_Cols> Mul(const MAT<T, L_Rows, L_Cols>& m1, const MAT<T, R_Rows, R_Cols>& m2) {
        MAT<T, L_Rows, R_Cols> m3;

        for (int col = 0; col < R_Cols; col++) {
            for (int row = 0; row < L_Rows; row++) {
                m3[col][row] = Dot(Row(m1, row), Column(m2, col));
            }
        }

        return m3;
    }

    template<template<typename T, int Rows, int Cols> class MAT, 
             typename T, int L_Rows, int L_Cols, 
                         int R_Rows,
             typename = std::enable_if<L_Cols == R_Rows>>
        inline Vector<T, R_Rows> Mul(const MAT<T, L_Rows, L_Cols>& m, const Vector<T, R_Rows>& v) {
        using MatrixAdapter = MAT<T, 1, R_Rows>;
        const MatrixAdapter& adapter = static_cast<const MatrixAdapter&>(v);

        MatrixAdapter result = Mul(m, adapter);
        return static_cast<const Vector<T, R_Rows>&>(result);
    }




    /////////////////////////////// QUATERNION
    template<template<typename T> class QUAT, typename T>
    T Dot(const QUAT<T>& q1, const QUAT<T>& q2) {
        Vector<T, 4> tmp(a.x*b.x, a.y*b.y, a.z*b.z, a.w*b.w);
        return tmp.x + tmp.y + tmp.z + tmp.w;
    }
  
}
