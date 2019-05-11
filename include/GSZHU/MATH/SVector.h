#pragma once

#include <assert.h>
#include <array>
#include <initializer_list>
#include <type_traits>

namespace GSZHU {
    template<typename T, int N>
    struct SVector : public std::array<T, N> {
        using BASE = std::array<T, N>;
        using ELEMENT = T;
        static constexpr int LENGTH = N;

        SVector() noexcept : BASE() {}
        explicit SVector(const T& Val) noexcept : BASE(Val) {}
        SVector(const T& V0, const T& V1, const T& V2 = 0, const T& V3 = 0) noexcept {
            BASE::operator[](0) = V0;
            BASE::operator[](1) = V1;
            if (LENGTH >= 3) BASE::operator[](2) = V2;
            if (LENGTH >= 4) BASE::operator[](3) = V3;
        }
        SVector(const std::initializer_list<T>& List) noexcept {
            auto Iter = List.begin();
            for (int i = 0; i < N; i++, ++Iter)
                BASE::operator[](i) = *Iter;
        }

        // V3(V4)
        template<int M, typename = std::enable_if_t<M >= N>> 
        SVector(const SVector<T, M>& Vec) noexcept {
            for (int i = 0; i < N; i++) 
                BASE::operator[](i) = Vec[i];
        }

        // V4(V3, 1.0)
        SVector(const SVector<T, N - 1>& Vec, const T& Val) noexcept {
            for (int i = 0; i < N - 1; i++)
                BASE::operator[](i) = Vec[i];
            BASE::operator[](N - 1) = Val;
        }

        inline T* GetData() noexcept { return (T*)this; }
        inline const T* GetData() const noexcept { return (const T*)this; }
        inline void SetData(const T* Src) {
            for (int i = 0; i < LENGTH; i++)
                BASE::operator[](i) = Src[i];
        }

        inline T& GetX() { return BASE::operator[](0); }
        inline T& GetY() { return BASE::operator[](1); }
        inline T& GetZ() { return BASE::operator[](2); }
        inline T& GetW() { return BASE::operator[](3); }
        inline const T& GetX() const { return BASE::operator[](0); }
        inline const T& GetY() const { return BASE::operator[](1); }
        inline const T& GetZ() const { return BASE::operator[](2); }
        inline const T& GetW() const { return BASE::operator[](3); }

        inline SVector operator+=(const SVector<T, N>& Vec) noexcept {
            for (int i = 0; i < LENGTH; i++)
                BASE::operator[](i) += Vec[i];
            return *this;
        }

        inline SVector operator-=(const SVector<T, N>& Vec) noexcept {
            for (int i = 0; i < LENGTH; i++)
                BASE::operator[](i) -= Vec[i];
            return *this;
        }

        inline SVector operator*=(const SVector<T, N>& Vec) noexcept {
            for (int i = 0; i < LENGTH; i++)
                BASE::operator[](i) *= Vec[i];
            return *this;
        }

        inline SVector operator/=(const SVector<T, N>& Vec) noexcept {
            for (int i = 0; i < LENGTH; i++)
                BASE::operator[](i) /= Vec[i];
            return *this;
        }

        template<typename U, typename = std::enable_if_t<std::is_arithmetic_v<U>>>
        inline SVector operator*=(const U& Val) noexcept {
            for (int i = 0; i < LENGTH; i++)
                BASE::operator[](i) *= Val;
            return *this;
        }

        template<typename U, typename = std::enable_if_t<std::is_arithmetic_v<U>>>
        inline SVector operator/=(const U& Val) noexcept {
            for (int i = 0; i < LENGTH; i++)
                BASE::operator[](i) /= Val;
            return *this;
        }
    };
}