#pragma once

#include "SVector.h"

namespace GSZHU {
    template<typename T>
    struct SQuaternion : public SVector<T, 4> {
        using BASE = SVector<T, 4>;
        using ELEMENT = T;
        static constexpr int LENGTH = 4;

        SQuaternion() noexcept : BASE() {}
        SQuaternion(const T& X, const T& Y, const T& Z, const T& W) noexcept : BASE(X, Y, Z, W) {}
        SQuaternion(const SVector<T, 3>& Vec, const T& W) noexcept : BASE(Vec[0], Vec[1], Vec[2], W) {}
        
        static SQuaternion Create(const T& Angle, const SVector<T, 3>& Vec) {
            
        }
    };
}