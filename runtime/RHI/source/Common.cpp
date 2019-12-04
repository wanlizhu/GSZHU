#include "Wanlix/RHI/Common.h"

namespace Wanlix
{
    Color::Color()
        : r(0.0f)
        , g(0.0f)
        , b(0.0f)
        , a(1.0f)
    {}

    Color::Color(float val)
        : r(val)
        , g(val)
        , b(val)
        , a(1.0f)
    {}

    Color::Color(int val)
        : r(val / 255.0f)
        , g(val / 255.0f)
        , b(val / 255.0f)
        , a(1.0f)
    {}

    Color::Color(float r, float g, float b, float a)
        : r(r)
        , g(g)
        , b(b)
        , a(a)
    {}

    Color::Color(int r, int g, int b, int a)
        : r(r / 255.0f)
        , g(g / 255.0f)
        , b(b / 255.0f)
        , a(a / 255.0f)
    {}

    Color::Color(std::initializer_list<float> init)
    {
        assert(init.size() >= 3);
        auto begin = init.begin();
        r = *begin;
        g = *(begin+1);
        b = *(begin+2);
        if (init.size() >= 4) {
            a = *(begin + 3);
        }
    }

    Color::Color(std::initializer_list<int> init)
    {
        assert(init.size() >= 3);
        auto begin = init.begin();
        r = *begin / 255.0f;
        g = *(begin + 1) / 255.0f;
        b = *(begin + 2) / 255.0f;
        if (init.size() >= 4) {
            a = *(begin + 3) / 255.0f;
        }
    }

    float& Color::operator[](int i)
    {
        return data[i];
    }

    const float& Color::operator[](int i) const
    {
        return data[i];
    }

    bool Color::operator==(const Color& rhs) const
    {
        return r == rhs.r &&
               g == rhs.g &&
               b == rhs.b &&
               a == rhs.a;
    }

    bool Color::operator!=(const Color& rhs) const
    {
        return !(*this == rhs);
    }

    Color::operator bool() const
    {
        return r >= 0 && g >= 0 && b >= 0;
    }
}