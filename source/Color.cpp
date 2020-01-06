#include "Wanlix/Color.h"

namespace Wanlix
{
    Color::Color()
        : _r(0)
        , _g(0)
        , _b(0)
        , _a(1)
    {}

    Color::Color(float r, float g, float b, float a)
        : _r(int(r * 255))
        , _g(int(g * 255))
        , _b(int(b * 255))
        , _a(int(a * 255))
    {}

    Color::Color(int r, int g, int b, int a)
        : _r(r)
        , _g(g)
        , _b(b)
        , _a(a)
    {}

    bool Color::IsOpaque() const
    {
        return _a == 255;
    }

    bool Color::operator==(Color const& rhs) const
    {
        return packed == rhs.packed;
    }

    bool Color::operator!=(Color const& rhs) const
    {
        return !(*this == rhs);
    }

    unsigned char& Color::operator[](int i)
    {
        return array[i];
    }

    unsigned char const& Color::operator[](int i) const
    {
        return array[i];
    }

    unsigned char& Color::r()
    {
        return _r;
    }

    unsigned char& Color::g()
    {
        return _g;
    }

    unsigned char& Color::b()
    {
        return _b;
    }

    unsigned char& Color::a()
    {
        return _a;
    }

    unsigned char const& Color::r() const
    {
        return _r;
    }

    unsigned char const& Color::g() const
    {
        return _g;
    }

    unsigned char const& Color::b() const
    {
        return _b;
    }

    unsigned char const& Color::a() const
    {
        return _a;
    }
}