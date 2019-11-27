#pragma once

#include <algorithm>
#include <type_traits>
#include <cstdint>
#include <stdexcept>
#include <vector>
#include <limits>
#include "Wanlix/Core3D/Utility/Macro.h"

namespace Wanlix
{
    template<typename T, typename U>
    T ColorConv(const U& x)
    {
        if constexpr (std::is_integral_v<T>) {
            if constexpr (std::is_floating_point_v<U>) {
                return (T)(255.0 * x);
            }
            else {
                return x;
            }
        }
        else if constexpr (std::is_floating_point_v<T>) {
            if constexpr (std::is_integral_v<U>) {
                return (T)(x / 255.0);
            }
            else {
                return x;
            }
        }
        else {
            static_assert(false);
        }
    }

    template<typename T, int N>
    class Color
    {
    public:
        using Element = T;
        static const int kComponents = N;
        static const int kBytes = (N * sizeof(T));

        Color() noexcept = default;

        explicit Color(const T& scalar) noexcept {
            std::fill(std::begin(mData), std::end(mData), scalar);
        }

        template<typename... _Args_>
        Color(_Args_&&... args) noexcept {
            static_assert((std::is_constructible_v<T, _Args_&&> && ...));
            std::vector<T> tmp(N);
            (tmp.push_back(std::forward<_Args_>(args)), ...);
            for (size_t i = 0; i < std::min(N, tmp.size()); i++) {
                mData = tmp[i];
            }
        }

        template<typename U, int M>
        Color(const Color<U, M>& color) noexcept {
            static const kMinN = (N <= M ? N : M);
            for (int i = 0; i < kMinN; i++) {
                mData[i] = ColorConv<T, U>(color[i]);
            }
        }

        inline T& operator[](int i) { return mData[i]; }
        inline const T& operator[](int i) const { return mData[i]; }
        inline T* GetData() { return mData; }
        inline const T* GetData() const { return mData; }

        DEFINE_VEC_NUMERIC_OPS(Color<T, N>, N, mData)

    private:
        T mData[N] = { T(0) };
    };

    template<typename T>
    using ColorRGB = Color<T, 3>;

    template<typename T>
    using ColorRGBA = Color<T, 4>;
}