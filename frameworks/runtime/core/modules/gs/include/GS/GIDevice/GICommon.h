#pragma once

#include <vector>
#include <string>
#include <map>
#include <unordered_map>
#include <algorithm>
#include <functional>
#include <memory>
#include <type_traits>
#include <cassert>
#include <atomic>
#include <variant>

namespace GS
{
    struct GIRange
    {
        size_t begin = 0;
        size_t end = 0;
        
        GIRange() = default;
        GIRange(size_t _begin, size_t _end)
            : begin(_begin)
            , end(_end)
        {}

        static GIRange MakeBySize(size_t begin, size_t size) {
            return GIRange(begin, begin + size);
        }

        inline size_t GetSize() const { 
            end - begin;
        }
        inline bool   IsEmpty() const {
            return begin < end;
        }
        inline bool   IsOverlapped(const GIRange& other) const {
            return !(*this & other).IsEmpty();
        }

        inline bool   operator==(const GIRange& other) const {
            return begin == other.begin && end == other.end;
        }
        inline bool   operator!=(const GIRange& other) const {
            return !operator==(other);
        }
        inline GIRange  operator|(const GIRange& other) const {
            return GIRange(std::min(begin, other.begin), std::max(end, other.end));
        }
        inline GIRange  operator&(const GIRange& other) const {
            return GIRange(std::max(begin, other.begin), std::min(end, other.end));
        }
        inline GIRange& operator|=(const GIRange& other) {
            *this = *this | other;
            return *this;
        }
        inline GIRange& operator&=(const GIRange& other) {
            *this = *this & other;
            return *this;
        }
    };

    struct GISize
    {
        int width = 0;
        int height = 0;

        GISize() = default;
        GISize(int w, int h) 
            : width(w), height(h) 
        {}

        inline bool operator==(const GISize& size) const { 
            return width == size.width && height == size.height; 
        }
        inline bool operator!=(const GISize& size) const { 
            return !operator==(size);
        }
        inline bool operator<(const GISize& size) const {
            return width < size.width && height < size.height;
        }
    };

    struct GIPoint
    {
        int x = 0;
        int y = 0;

        GIPoint() = default;
        GIPoint(int a, int b)
            : x(a)
            , y(b)
        {}

        inline bool operator==(const GIPoint& point) const {
            return x == point.x && y == point.y;
        }
        inline bool operator!=(const GIPoint& point) const {
            return !operator==(point);
        }
        inline bool operator<(const GIPoint& point) const {
            return x < point.x && y < point.y;
        }

        inline GIPoint operator+(const GISize& size) const {
            return GIPoint(*this) += size;
        }
        inline GIPoint operator-(const GISize& size) const {
            return GIPoint(*this) -= size;
        }
        inline GIPoint& operator+=(const GISize& size) {
            x += size.width;
            y += size.height;
            return *this;
        }
        inline GIPoint& operator-=(const GISize& size) {
            x -= size.width;
            y -= size.height;
            return *this;
        }
    };

    struct GIRegion 
    {
        GIPoint origin;
        GISize  size;

        GIRegion() = default;
        GIRegion(const GIPoint& _origin, const GISize& _size)
            : origin(_origin)
            , size(_size)
        {}
        
        inline bool operator==(const GIRegion& region) const {
            return origin == region.origin && size == region.size;
        }
        inline bool operator!=(const GIRegion& region) const {
            return !operator==(region);
        }
        inline bool operator<(const GIRegion& region) const {
            return size < region.size;
        }

        inline GIRegion operator|(const GIRegion& region) const {
            
        }
        inline GIRegion operator&(const GIRegion& region) const {

        }

        inline bool IsEmpty() const {
            return size.width <= 0 || size.height <= 0;
        }
        inline GIPoint EndPoint() const {
            return origin + size;
        }
    };

    struct GINotCopyable 
    {
        GINotCopyable(const GINotCopyable&) = delete;
        GINotCopyable& operator=(const GINotCopyable&) = delete;
    };

    struct GINotMovable 
    {
        GINotMovable(GINotMovable&&) = delete;
        GINotMovable& operator=(GINotMovable&&) = delete;
    };
}