#pragma once

#include <string>
#include <vector>
#include <unordered_map>
#include <algorithm>
#include <cstdint>
#include <atomic>
#include <memory>

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
        INT2(int val) : x(val), y(val) {}
        INT2(int x, int y) : x(x), y(y) {}

        bool operator==(const INT2& rhs) const {
            return x == rhs.x && y == rhs.y;
        }

        bool operator!=(const INT2& rhs) const {
            return !(*this == rhs);
        }

        bool operator<(const INT2& rhs) const {
            return x < rhs.x && y < rhs.y;
        }

        bool operator<=(const INT2& rhs) const {
            return x <= rhs.x && y <= rhs.y;
        }

        bool operator>(const INT2& rhs) const {
            return x > rhs.x && y > rhs.y;
        }

        bool operator>=(const INT2& rhs) const {
            return x >= rhs.x && y >= rhs.y;
        }
    };
}