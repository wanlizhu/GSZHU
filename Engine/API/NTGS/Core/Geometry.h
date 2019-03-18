#pragma once

#include "BasicTypes.h"

namespace NTGS {
    /////////////////////////////// VECTOR
    template<typename VEC>
    inline typename VEC::ElementType Dot(const VEC& v1, const VEC& v2) {
        typename VEC::ElementType sum = 0;
        for (int i = 0; i < VEC::DIMENSION; i++) {
            sum += v1[i] * v2[i];
        }
        return sum;
    }

    template<typename VEC,
             typename = std::enable_if<VEC::DIMENSION == 3>>
    inline VEC Cross(const VEC& v1, const VEC& v2) {
        return VEC(
            v1.y * v2.z - v2.y * v1.z,
            v1.z * v2.x - v2.z * v1.x,
            v1.x * v2.y - v2.x * v1.y);
    }

    template<typename VEC>
    inline typename VEC::ElementType Length(const VEC& v) {
        typename VEC::ElementType sum = 0;
        for (int i = 0; i < VEC::DIMENSION; i++) {
            sum += v[i] * v[i];
        }
        return std::sqrt(sum);
    }

    template<typename VEC>
    inline typename VEC::ElementType Distance(const VEC& v1, const VEC& v2) {
        return Length(v2 - v1);
    }

    template<typename VEC>
    inline VEC Normalize(const VEC& v) {
        if (v.IsNormalized())
            return v;
        else{
            VEC inst(v);
            typename VEC::ElementType len = Length(v);
            for (int i = 0; i < VEC::DIMENSION; i++) {
                inst[i] /= len;
            }
            return inst;
        }
    }

    template<typename VEC>
    inline VEC Reflect(const VEC& in, const VEC& normal) {
        using E = typename VEC::ElementType;
        return in - normal * Dot(normal, in) * E(2);
    }

    template<typename VEC>
    inline VEC Refract(const VEC& in, const VEC& normal, typename VEC::ElementType eta) {
        using E = typename VEC::ElementType;
        const E dotValue = Dot(normal, in);
        const E k = E(1) - eta * eta * (E(1) - dotValue * dotValue);
        return (eta * in - (eta * dotValue + std::sqrt(k)) * normal) * E(k >= E(0));
    }

    template<typename VEC>
    inline VEC ConvertLinearToSRGB(const VEC& colorLinear, typename VEC::ElementType gamma) {

    }

    template<typename VEC>
    inline VEC ConvertSRGBToLinear(const VEC& colorSRGB, typename VEC::ElementType gamma) {

    }

    template<typename VEC, 
             typename = std::enable_if<VEC::DIMENSION == 2 || VEC::DIMENSION == 3>>
    inline VEC ClosestPoint(const VEC& point, const Line<VEC>& line) {
        
    }



    /////////////////////////////// MATRIX
    template<typename M>
    inline typename M::TransposeType Transpose(const M& m) {
        typename M::TransposeType result;
        for (int i = 0; i < M::COLS; i++) {
            for (int j = 0; j < M::ROWS; j++) {
                result[j][i] = m[i][j];
            }
        }
        return result;
    }

    template<typename M>
    inline typename M::ScalarType Determinant(const M& m) {
        
    }

    template<typename M,
             typename = std::enable_if<M::ROWS == M::COLS>>
    inline M Inverse(const M& m) {
        
    }

    template<typename M1, typename M2,
             typename = std::enable_if<M1::COLS == M2::ROWS>>
    inline Matrix<typename M1::ElementType, M1::ROWS, M2::COLS>
        Mul(const M1& m1, const M2& m2) {
    
    }

    template<typename M, typename VEC,
             typename = std::enable_if<M::COLS == VEC::DIMENSION>>
        inline VEC Mul(const M& m, const VEC& v) {
        using VecAsMat = Matrix<typename VEC::ElementType, VEC::DIMENSION, 1>;
        return Mul(m, static_cast<const VecAsMat&>(v));
    }




    /////////////////////////////// QUATERNION

}
