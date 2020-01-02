#include "Wanlix/RHI/Common.h"

namespace Wanlix
{
    Color4i::Color4i()
        : r(0)
        , g(0)
        , b(0)
        , a(255)
    {}

    Color4i::Color4i(Int val)
        : r(std::clamp<Byte>(val, 0, 255))
        , g(std::clamp<Byte>(val, 0, 255))
        , b(std::clamp<Byte>(val, 0, 255))
        , a(255)
    {}

    Color4i::Color4i(Int r, Int g, Int b, Int a)
        : r(std::clamp<Byte>(r, 0, 255))
        , g(std::clamp<Byte>(g, 0, 255))
        , b(std::clamp<Byte>(b, 0, 255))
        , a(std::clamp<Byte>(a, 0, 255))
    {}

    Color4i::Color4i(std::initializer_list<Int> init)
        : Color4i()
    {
        assert(init.size() >= 3);
        Int i = 0;
        for (auto& it : init) {
            if (i < 4) {
                data[i++] = std::clamp<Byte>(it, 0, 255);
            }
        }
    }

    Color4i::Color4i(Color4f const& rhs)
        : Color4i()
    {
        for (int i = 0; i < 4; i++) {
            data[i] = (Byte)(rhs.data[i] * 255.f);
            data[i] = std::clamp<Byte>(data[i], 0, 255);
        }
    }

    Color4i& Color4i::operator=(Color4f const& rhs)
    {
        for (int i = 0; i < 4; i++) {
            data[i] = (Byte)(rhs.data[i] * 255.f);
            data[i] = std::clamp<Byte>(data[i], 0, 255);
        }
    }

    Byte& Color4i::operator[](Int i)
    {
        return data[i];
    }

    const Byte& Color4i::operator[](Int i) const
    {
        return data[i];
    }

    Bool Color4i::operator==(const Color4i& rhs) const
    {
        return r == rhs.r &&
               g == rhs.g &&
               b == rhs.b &&
               a == rhs.a;
    }

    Bool Color4i::operator!=(const Color4i& rhs) const
    {
        return !(*this == rhs);
    }

    Color4f::Color4f()
        : r(0)
        , g(0)
        , b(0)
        , a(255)
    {}

    Color4f::Color4f(float val)
        : r(std::clamp<float>(val, 0.f, 1.f))
        , g(std::clamp<float>(val, 0.f, 1.f))
        , b(std::clamp<float>(val, 0.f, 1.f))
        , a(1.f)
    {}

    Color4f::Color4f(float r, float g, float b, float a)
        : r(std::clamp<float>(r, 0.f, 1.f))
        , g(std::clamp<float>(g, 0.f, 1.f))
        , b(std::clamp<float>(b, 0.f, 1.f))
        , a(std::clamp<float>(a, 0.f, 1.f))
    {}

    Color4f::Color4f(std::initializer_list<float> init)
        : Color4f()
    {
        assert(init.size() >= 3);
        Int i = 0;
        for (auto& it : init) {
            if (i < 4) {
                data[i++] = std::clamp<float>(it, 0.f, 1.f);
            }
        }
    }

    Color4f::Color4f(Color4i const& rhs)
        : Color4f()
    {
        for (int i = 0; i < 4; i++) {
            data[i] = (float)(rhs.data[i] / 255.f);
            data[i] = std::clamp<float>(data[i], 0.f, 1.f);
        }
    }

    Color4f& Color4f::operator=(Color4i const& rhs)
    {
        for (int i = 0; i < 4; i++) {
            data[i] = (float)(rhs.data[i] / 255.f);
            data[i] = std::clamp<float>(data[i], 0.f, 1.f);
        }
    }

    float& Color4f::operator[](Int i)
    {
        return data[i];
    }

    float const& Color4f::operator[](Int i) const
    {
        return data[i];
    }

    Bool Color4f::operator==(const Color4f& rhs) const
    {
        static const float eps = 1e-5;
        for (int i = 0; i < 4; i++) {
            if (!Equals(data[i], rhs.data[i]), eps) {
                return false;
            }
        }
        return true;
    }

    Bool Color4f::operator!=(const Color4f& rhs) const
    {
        return !(*this == rhs);
    }
}