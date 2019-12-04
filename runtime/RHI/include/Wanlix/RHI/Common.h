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
        int offset = 0;
        int size = 0;
    };

    struct Offset
    {
        int x = 0;
        int y = 0;
        int z = 0;
    };

    struct Extent
    {
        int width = 0;
        int height = 0;
        int depth = 1;
    };

    struct Region
    {
        Offset offset;
        Extent extent;
    };

    struct SubresourceOffset
    {
        int    arrayLayer = 0;
        int    mipLevel = 0;
        Offset offset;
    };

    struct SubresourceExtent
    {
        int    numArrayLayers = 1;
        int    numMipLevels = 1;
        Extent extent;
    };

    struct SubresourceRegion
    {
        SubresourceOffset offset;
        SubresourceExtent extent;
    };

    struct Color
    {
        union {
            struct { float r, g, b, a; };
            float data[4];
        };

        Color();
        Color(float val);
        Color(int val);
        Color(float r, float g, float b, float a = 1.0f);
        Color(int r, int g, int b, int a = 255);
        Color(std::initializer_list<float> init);
        Color(std::initializer_list<int> init);

        float&       operator[](int i);
        const float& operator[](int i) const;
        bool operator==(const Color& rhs) const;
        bool operator!=(const Color& rhs) const;

        operator bool() const;
    };
}