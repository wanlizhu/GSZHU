#pragma once

#include "Core/Config.h"
#include "Core/Common.h"

namespace Wanlix
{
    class Color
    {
    public:
        constexpr Color() 
            : packed(0)
        {}
        constexpr Color(float r, float g, float b, float a = 1.f)
            : _r((int)std::clamp(r * 255.f, 0.f, 255.f))
            , _g((int)std::clamp(g * 255.f, 0.f, 255.f))
            , _b((int)std::clamp(b * 255.f, 0.f, 255.f))
            , _a((int)std::clamp(a * 255.f, 0.f, 255.f))
        {}
        constexpr Color(int r, int g, int b, int a = 255)
            : _r(r), _g(g), _b(b), _a(a)
        {}

        FORCEINLINE bool IsOpaque() const { return _a == 255; }
        FORCEINLINE bool operator==(Color const& rhs) const { return packed == rhs.packed; }
        FORCEINLINE bool operator!=(Color const& rhs) const { return !(*this == rhs); }
        FORCEINLINE unsigned char& operator[](int i) { return array[i]; }
        FORCEINLINE unsigned char const& operator[](int i) const { return array[i]; }

        FORCEINLINE unsigned char& r() { return _r; }
        FORCEINLINE unsigned char& g() { return _g; }
        FORCEINLINE unsigned char& b() { return _b; }
        FORCEINLINE unsigned char& a() { return _a; }
        FORCEINLINE unsigned char const& r() const { return _r; }
        FORCEINLINE unsigned char const& g() const { return _g; }
        FORCEINLINE unsigned char const& b() const { return _b; }
        FORCEINLINE unsigned char const& a() const { return _a; }

    private:
        union {
            struct { unsigned char _r, _g, _b, _a; };
            unsigned int packed;
            unsigned char array[4];
        };
    };
}