#pragma once

#include <string>
#include <vector>
#include <unordered_map>
#include <algorithm>
#include <cstdint>

namespace Wanlix
{
    struct INT2
    {
        union {
            struct {
                int x;
                int y;
            };
            struct {
                int width;
                int height;
            };
        };
        INT2() : x(0), y(0) {}
    };
}