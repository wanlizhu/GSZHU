#pragma once

#include <cstdint>
#include <type_traits>

namespace Wanlix
{
    struct Range 
    {
        uint32_t begin = 0;
        uint32_t size = 0;

        Range() = default;
        Range(
            uint32_t begin,
            uint32_t size
        )
            : begin(begin)
            , size(size)
        {}

        bool IsEmpty() const;
        Range operator+(uint32_t count) const;
        Range operator-(uint32_t count) const;
        bool operator==(const Range& other) const;
        bool operator!=(const Range& other) const;
    };

    struct Offset
    {
        int x = 0;
        int y = 0;
        int z = 0;

        Offset() = default;
        Offset(
            int x,
            int y,
            int z = 0
        )
            : x(x)
            , y(y)
            , z(z)
        {}

        bool IsZero() const;
        Offset operator+(const Offset& other) const;
        Offset operator-(const Offset& other) const;
        bool operator==(const Offset& other) const;
        bool operator!=(const Offset& other) const;
    };

    struct Extent
    {
        uint32_t width = 0;
        uint32_t height = 0;
        uint32_t depth = 0;

        Extent() = default;
        Extent(
            uint32_t width,
            uint32_t height,
            uint32_t depth = 0
        )
            : width(width)
            , height(height)
            , depth(depth)
        {}

        bool IsEmpty() const;
        Extent operator+(const Extent& other) const;
        Extent operator-(const Extent& other) const;
        bool operator==(const Extent& other) const;
        bool operator!=(const Extent& other) const;
    };

    struct Region
    {
        Offset offset;
        Extent extent;

        Region() = default;
        Region(
            const Offset& offset, 
            const Extent& extent
        )
            : offset(offset)
            , extent(extent)
        {}

        bool IsEmpty() const;
        Region operator+(const Extent& ext) const;
        Region operator-(const Extent& ext) const;
        bool operator==(const Region& other) const;
        bool operator!=(const Region& other) const;
    };
}