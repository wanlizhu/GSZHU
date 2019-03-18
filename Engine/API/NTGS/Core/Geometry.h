#pragma once

#include "BasicTypes.h"

namespace NTGS {
    template<typename T>
    typename T::ElementType Dot(const T& v1, const T& v2) {
        typename T::ElementType sum = 0;
        for (int i = 0; i < T::DIMENSION; i++) {
            sum += v1[i] * v2[i];
        }
        return sum;
    }

    template<typename T, typename=std::enable_if<T::DIMENSION==3>>
    T Cross(const T& v1, const T& v2) {
        return T(
            v1.y * v2.z - v2.y * v1.z,
            v1.z * v2.x - v2.z * v1.x,
            v1.x * v2.y - v2.x * v1.y);
    }

    template<typename T>
    typename T::ElementType Length(const T& v) {
        typename T::ElementType sum = 0;
        for (int i = 0; i < T::DIMENSION; i++) {
            sum += v[i] * v[i];
        }
        return std::sqrt(sum);
    }

    template<typename T>
    T Normalize(const T& v) {
        if (v.IsNormalized())
            return v;
        else{
            T inst(v);
            typename T::ElementType len = Length(v);
            for (int i = 0; i < T::DIMENSION; i++) {
                inst[i] /= len;
            }
            return inst;
        }
    }
}