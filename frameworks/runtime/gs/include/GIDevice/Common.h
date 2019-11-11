#pragma once

#include <vector>
#include <string>
#include <map>
#include <unordered_map>
#include <algorithm>
#include <functional>
#include <memory>
#include <type_traits>

namespace GS2::GI
{
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

        inline bool empty() const {
            return size.width <= 0 || size.height <= 0;
        }
        inline Point endPoint() const {
            return origin + size;
        }
    };
}