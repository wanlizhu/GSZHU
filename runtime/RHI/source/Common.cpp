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
        : r(std::clamp(val, -1.0f, 1.0f))
        , g(std::clamp(val, -1.0f, 1.0f))
        , b(std::clamp(val, -1.0f, 1.0f))
        , a(1.0f)
    {}

    Color::Color(uint32_t val)
        : r(std::clamp(val / 255.0f, -1.0f, 1.0f))
        , g(std::clamp(val / 255.0f, -1.0f, 1.0f))
        , b(std::clamp(val / 255.0f, -1.0f, 1.0f))
        , a(1.0f)
    {}

    Color::Color(float r, float g, float b, float a)
        : r(std::clamp(r, -1.0f, 1.0f))
        , g(std::clamp(g, -1.0f, 1.0f))
        , b(std::clamp(b, -1.0f, 1.0f))
        , a(std::clamp(a, -1.0f, 1.0f))
    {}

    Color::Color(uint32_t r, uint32_t g, uint32_t b, uint32_t a)
        : r(std::clamp(r / 255.0f, -1.0f, 1.0f))
        , g(std::clamp(g / 255.0f, -1.0f, 1.0f))
        , b(std::clamp(b / 255.0f, -1.0f, 1.0f))
        , a(std::clamp(a / 255.0f, -1.0f, 1.0f))
    {}

    Color::Color(std::initializer_list<float> init)
        : Color()
    {
        assert(init.size() >= 3);
        int i = 0;
        for (auto& it : init) {
            if (i < 4) {
                data[i++] = std::clamp(it, -1.0f, 1.0f);
            }
        }
    }

    Color::Color(std::initializer_list<uint32_t> init)
        : Color()
    {
        assert(init.size() >= 3);
        int i = 0;
        for (auto& it : init) {
            if (i < 4) {
                data[i++] = std::clamp(it / 255.0f, -1.0f, 1.0f);
            }
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
        return r >= 0.0f && g >= 0.0f && b >= 0.0f && a >= 0.0f;
    }
}