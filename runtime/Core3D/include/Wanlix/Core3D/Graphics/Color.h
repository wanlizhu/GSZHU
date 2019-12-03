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
    void ConvColorChannel(T* dst, const U& x)
    {
        if (x < 0) {
            *dst = T(-1);
            return;
        }

        if constexpr (std::is_integral_v<T>) {
            if constexpr (std::is_floating_point_v<U>) {
                *dst = (T)(255.0 * x);
            }
            else {
                *dst = (T)x;
            }
        }
        else if constexpr (std::is_floating_point_v<T>) {
            if constexpr (std::is_integral_v<U>) {
                *dst = (T)(x / 255.0);
            }
            else {
                *dst = (T)x;
            }
        }
        else {
            static_assert(false);
        }
    }

    struct ColorData
    {
        std::shared_ptr<char[]> data;
        ColorFormat format;
        DataType dataType;

        ColorData(
            const ColorFormat& format,
            const DataType& dataType
        )
            : data(std::shared_ptr<char[]>(new char[ColorFormatSize(format) * DataTypeSize(dataType)]))
            , format(format)
            , dataType(dataType)
        {}

        const char& operator[](int i) const { return (data.get())[i]; }
        char&       operator[](int i)       { return (data.get())[i]; }
    };

    template<typename T, int N>
    class Color
    {
    public:
        static Color Null()  { return Color4B(-1, -1, -1, -1); }
        static Color White() { return Color4B(1, 1, 1, 1); }
        static Color Red()   { return Color4B(1, 0, 0, 1); }
        static Color Green() { return Color4B(0, 1, 0, 1); }
        static Color Blue()  { return Color4B(0, 0, 1, 1); }
        static Color Black() { return Color4B(0, 0, 0, 1); }

        Color() noexcept = default;

        explicit Color(const T& scalar) noexcept {
            for (int i = 0; i < N; i++) {
                mData[i] = scalar;
            }
        }

        template<typename U>
        Color(const std::initializer_list<U>& init) noexcept {
            assert(init.size() >= N);
            auto it = init.begin();
            for (int i = 0; i < N; i++, it++) {
                mData[i] = static_cast<T>(*it);
            }
        }

        template<typename U>
        Color(U r, U g, U b, U a = 1) noexcept {
            if (N >= 1) mData[0] = static_cast<T>(r);
            if (N >= 2) mData[1] = static_cast<T>(g);
            if (N >= 3) mData[2] = static_cast<T>(b);
            if (N >= 4) mData[3] = static_cast<T>(a);
        }

        template<typename U, int M>
        Color(const Color<U, M>& color) noexcept {
            static const kMinN = (N <= M ? N : M);
            for (int i = 0; i < kMinN; i++) {
                ConvColorChannel<T, U>(&mData[i], color[i]);
            }
        }

        T&       operator[](int i)       { return mData[i]; }
        const T& operator[](int i) const { return mData[i]; }

        bool IsNull() const noexcept
        { 
            for (int i = 0; i < N; i++) {
                if (mData[i] < 0)
                    return true;
            }
            return false;
        }

        ColorData Convert(
            const ColorFormat& format,
            const DataType& dataType
        ) const noexcept
        {
            ColorData data(format, dataType);
            const size_t elementSize = DataTypeSize(dataType);
            for (int i = 0; i < ColorFormatSize(format); i++) {
                char* dst = &data[i * DataTypeSize(dataType)];
                std::memcpy(dst, );
            }
            return data;
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