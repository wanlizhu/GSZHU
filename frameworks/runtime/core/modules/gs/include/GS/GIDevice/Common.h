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

namespace GS::GI
{
    struct Range
    {
        size_t begin = 0;
        size_t end = 0;
        
        Range() = default;
        Range(size_t _begin, size_t _end)
            : begin(_begin)
            , end(_end)
        {}

        static Range MakeBySize(size_t begin, size_t size) {
            return Range(begin, begin + size);
        }

        inline size_t GetSize() const { 
            end - begin;
        }
        inline bool   IsEmpty() const {
            return begin < end;
        }
        inline bool   IsOverlapped(const Range& other) const {
            return !(*this & other).IsEmpty();
        }

        inline bool   operator==(const Range& other) const {
            return begin == other.begin && end == other.end;
        }
        inline bool   operator!=(const Range& other) const {
            return !operator==(other);
        }
        inline Range  operator|(const Range& other) const {
            return Range(std::min(begin, other.begin), std::max(end, other.end));
        }
        inline Range  operator&(const Range& other) const {
            return Range(std::max(begin, other.begin), std::min(end, other.end));
        }
        inline Range& operator|=(const Range& other) {
            *this = *this | other;
            return *this;
        }
        inline Range& operator&=(const Range& other) {
            *this = *this & other;
            return *this;
        }
    };

    struct Size
    {
        int width = 0;
        int height = 0;

        Size() = default;
        Size(int w, int h) 
            : width(w), height(h) 
        {}

        inline bool operator==(const Size& size) const { 
            return width == size.width && height == size.height; 
        }
        inline bool operator!=(const Size& size) const { 
            return !operator==(size);
        }
        inline bool operator<(const Size& size) const {
            return width < size.width && height < size.height;
        }
    };

    struct Point
    {
        int x = 0;
        int y = 0;

        Point() = default;
        Point(int a, int b)
            : x(a)
            , y(b)
        {}

        inline bool operator==(const Point& point) const {
            return x == point.x && y == point.y;
        }
        inline bool operator!=(const Point& point) const {
            return !operator==(point);
        }
        inline bool operator<(const Point& point) const {
            return x < point.x && y < point.y;
        }

        inline Point operator+(const Size& size) const {
            return Point(*this) += size;
        }
        inline Point operator-(const Size& size) const {
            return Point(*this) -= size;
        }
        inline Point& operator+=(const Size& size) {
            x += size.width;
            y += size.height;
            return *this;
        }
        inline Point& operator-=(const Size& size) {
            x -= size.width;
            y -= size.height;
            return *this;
        }
    };

    struct Region 
    {
        Point origin;
        Size  size;

        Region() = default;
        Region(const Point& _origin, const Size& _size)
            : origin(_origin)
            , size(_size)
        {}
        
        inline bool operator==(const Region& region) const {
            return origin == region.origin && size == region.size;
        }
        inline bool operator!=(const Region& region) const {
            return !operator==(region);
        }
        inline bool operator<(const Region& region) const {
            return size < region.size;
        }

        inline Region operator|(const Region& region) const {
            
        }
        inline Region operator&(const Region& region) const {

        }

        inline bool IsEmpty() const {
            return size.width <= 0 || size.height <= 0;
        }
        inline Point EndPoint() const {
            return origin + size;
        }
    };
}