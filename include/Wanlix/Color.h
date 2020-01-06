#pragma once

#include "Wanlix/Common.h"

namespace Wanlix
{
    class Color
    {
    public:
        Color();
        Color(float r, float g, float b, float a = 1.f);
        Color(int r, int g, int b, int a = 255);

        bool IsOpaque() const;
        bool operator==(Color const& rhs) const;
        bool operator!=(Color const& rhs) const;
        unsigned char& operator[](int i);
        unsigned char const& operator[](int i) const;

        unsigned char& r();
        unsigned char& g();
        unsigned char& b();
        unsigned char& a();
        unsigned char const& r() const;
        unsigned char const& g() const;
        unsigned char const& b() const;
        unsigned char const& a() const;

    private:
        union {
            struct { unsigned char _r, _g, _b, _a; };
            unsigned int packed;
            unsigned char array[4];
        };
    };
}