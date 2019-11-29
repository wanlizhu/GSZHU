#pragma once

#include <algorithm>
#include <type_traits>
#include <cstdint>
#include <stdexcept>
#include <cassert>
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
        static const int kBytes      = (N * sizeof(T));

        static Color Null() {
            return Color(-1);
        }

        Color() noexcept = default;

        explicit Color(const T& scalar) noexcept {
            std::fill(std::begin(mData), std::end(mData), scalar);
        }

        template<typename U>
        Color(const std::initializer_list<U>& init) noexcept {
            int i = 0;
            for (auto& val : init) {
                if (i < N) {
                    mData[i++] = static_cast<T>(val);
                }
            }
        }

        Color(T r, T g, T b, T a) noexcept {
            if (N >= 1) mData[0] = r;
            if (N >= 2) mData[1] = g;
            if (N >= 3) mData[2] = b;
            if (N >= 4) mData[3] = a;
        }

        template<typename U, int M>
        Color(const Color<U, M>& color) noexcept {
            static const kMinN = (N <= M ? N : M);
            for (int i = 0; i < kMinN; i++) {
                mData[i] = ColorConv<T, U>(color[i]);
            }
        }

        inline T&       operator[](int i)       { return mData[i]; }
        inline const T& operator[](int i) const { return mData[i]; }
        inline T*       GetData()               { return mData; }
        inline const T* GetData() const         { return mData; }
        inline bool     IsNull() const { 
            return std::any_of(&mData[0], &mData[0] + N,
                               [](T val) { 
                                   return val < 0;
                               }); 
        }

        DEFINE_VEC_NUMERIC_OPS(Color, N, mData)

    private:
        T mData[N] = { T(0) };
    };

    template<typename T>
    using ColorRGB = Color<T, 3>;

    template<typename T>
    using ColorRGBA = Color<T, 4>;

    using Color4F = ColorRGBA<float>;
    using Color4B = ColorRGBA<char>;
    using Color3F = ColorRGB<float>;
    using Color3B = ColorRGB<char>;
}