#pragma once

#include "SVector.h"

namespace GSZHU {
    template<typename T, int N>
    inline T LengthSquared(const SVector<T, N>& Vec) noexcept {
        T Sum = 0;
        for (int i = 0; i < N; i++)
            Sum += Vec[i] * Vec[i];
        return Sum;
    }

    template<typename T, int N>
    inline T Length(const SVector<T, N>& Vec) noexcept {
        return sqrt(LengthSquared(Vec));
    }

    template<typename T, int N>
    inline SVector<T, N> Normalize(const SVector<T, N>& Vec) noexcept {
        return SVector<T, N>(Vec) /= Length(Vec);
    }

    // -A
    template<typename T, int N>
    inline SVector<T, N> operator-(const SVector<T, N>& V) {
        SVector<T, N> Ret;
        for (int i = 0; i < N; i++)
            Ret[i] = -V[i];
        return Ret;
    }

    // A + B
    template<typename T, int N>
    inline SVector<T, N> operator+(const SVector<T, N>& V1, const SVector<T, N>& V2) {
        return SVector<T, N>(V1) += V2;
    }

    // A - B
    template<typename T, int N>
    inline SVector<T, N> operator-(const SVector<T, N>& V1, const SVector<T, N>& V2) {
        return SVector<T, N>(V1) -= V2;
    }

    // A * B
    template<typename T, int N>
    inline SVector<T, N> operator*(const SVector<T, N>& V1, const SVector<T, N>& V2) {
        return SVector<T, N>(V1) *= V2;
    }

    // A / B
    template<typename T, int N>
    inline SVector<T, N> operator/(const SVector<T, N>& V1, const SVector<T, N>& V2) {
        return SVector<T, N>(V1) /= V2;
    }

    // 2 * A
    template<typename T, int N>
    inline SVector<T, N> operator*(const T& Val, const SVector<T, N>& V2) {
        return V2 * Val;
    }

    // A * 2
    template<typename T, int N>
    inline SVector<T, N> operator*(const SVector<T, N>& V1, const T& Val) {
        return SVector<T, N>(V1) *= Val;
    }

    // A / 2
    template<typename T, int N>
    inline SVector<T, N> operator/(const SVector<T, N>& V1, const T& Val) {
        return SVector<T, N>(V1) /= Val;
    }

    // A dot B 
    template<typename T, int N>
    inline T Dot(const SVector<T, N>& V1, const SVector<T, N>& V2) noexcept {
        T Sum = 0;
        for (int i = 0; i < N; i++)
            Sum += V1[i] * V2[i];
        return Sum;
    }

    // A cross B
    template<typename T>
    inline SVector<T, 3> Cross(const SVector<T, 3>& V1, const SVector<T, 3>& V2) noexcept {
        return SVector<T, 3>(
            V1.GetY() * V2.GetZ() - V2.GetY() * V1.GetZ(),
            V1.GetZ() * V2.GetX() - V2.GetZ() * V1.GetX(),
            V1.GetX() * V2.GetY() - V2.GetX() * V1.GetY());
    }
}