#pragma once

#include <cstdint>
#include <type_traits>
#include <cassert>
#include <algorithm>
#include <unordered_map>
#include <memory>
#include <typeinfo>
#include <typeindex>
#include <functional>
#include <string>
#include <vector>
#include <queue>
#include <deque>
#include <list>
#include <codecvt>
#include <chrono>
#include <filesystem>
#include <fstream>
#include <sstream>
#include <iostream>
#include <any>
#include <tuple>
#include <variant>
#include <thread>
#include <mutex>
#include <condition_variable>
#include "Config.h"
#include "Format.h"
#include "NonCopyable.h"

namespace Wanlix
{
    struct Range
    {
        uint32_t offset = 0;
        uint32_t size = 0;
    };

    struct Offset
    {
        uint32_t x = 0;
        uint32_t y = 0;
        uint32_t z = 0;
    };

    struct Extent
    {
        uint32_t width = 0;
        uint32_t height = 0;
        uint32_t depth = 1;
    };

    struct Region
    {
        Offset offset;
        Extent extent;
    };

    struct Subresource
    {
        uint32_t baseArrayLayer = 0;
        uint32_t numArrayLayers = 1;
        uint32_t baseMipLevel = 0;
        uint32_t numMipLevels = 1;
    };

    struct SwizzleRGBA
    {
        uint8_t r = 0;
        uint8_t g = 1;
        uint8_t b = 2;
        uint8_t a = 3;
    };

    struct Color
    {
        union {
            struct { float r, g, b, a; };
            float data[4];
        };

        Color();
        Color(float val);
        Color(uint32_t val);
        Color(float r, float g, float b, float a = 1.0f);
        Color(uint32_t r, uint32_t g, uint32_t b, uint32_t a = 255);
        Color(std::initializer_list<float> init);
        Color(std::initializer_list<uint32_t> init);

        float&       operator[](int i);
        const float& operator[](int i) const;
        bool operator==(const Color& rhs) const;
        bool operator!=(const Color& rhs) const;
        operator bool() const;
    };
}