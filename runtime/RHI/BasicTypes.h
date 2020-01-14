#pragma once

#include "Core/Common.h"

namespace Wanlix
{
    struct Range
    {
        static const Uint kMax = std::numeric_limits<Uint>::max();
        Uint offset = 0;
        Uint size = kMax;

        static const Range& All() 
        {
            static const Range all(0, kMax);
            return all;
        }

        Range() = default;
        Range(Uint offset, Uint size = kMax)
            : offset(offset)
            , size(size)
        {}

        inline bool IsEmpty() const
        {
            return size == 0;
        }

        inline bool IsAll() const
        {
            return *this == All();
        }

        inline bool operator==(const Range& rhs) const
        {
            return offset == rhs.offset && size == rhs.size;
        }
    };

    struct Extent
    {
        static const Uint kMax = std::numeric_limits<Uint>::max();
        Uint width = kMax;
        Uint height = kMax;

        static const Extent& All()
        {
            static const Extent all(kMax, kMax);
            return all;
        }

        Extent() = default;
        Extent(Uint w, Uint h)
            : width(w)
            , height(h)
        {}

        inline bool IsEmpty() const
        {
            return width == 0 && height == 0;
        }

        inline bool IsAll() const
        {
            return *this == All();
        }

        inline bool operator==(const Extent& rhs) const
        {
            return width == rhs.width && height == rhs.height;
        }
    };

    struct Offset
    {
        static const Uint kMax = std::numeric_limits<Uint>::max();
        Uint x = 0;
        Uint y = 0;

        static const Offset& Zero()
        {
            static const Offset zero(0, 0);
            return zero;
        }

        Offset() = default;
        Offset(Uint x, Uint y)
            : x(x)
            , y(y)
        {}

        inline bool IsZero() const
        {
            return *this == Zero();
        }

        inline bool operator==(const Offset& rhs) const
        {
            return x == rhs.x && y == rhs.y;
        }

        inline Offset operator+(const Extent& extent) const
        {
            return Offset(x + extent.width, y + extent.height);
        }

        inline Offset& operator+=(const Extent& extent)
        {
            *this = *this + extent;
            return *this;
        }
    };

    struct Region
    {
        Offset offset;
        Extent extent;

        Region() = default;
        Region(const Offset& offset, const Extent& extent)
            : offset(offset)
            , extent(extent)
        {}

        inline bool IsEmpty() const 
        {
            return extent.IsEmpty();
        }

        inline bool IsAll() const
        {
            return offset.IsZero() && extent.IsAll();
        }

        inline bool operator==(const Region& rhs) const
        {
            return offset == offset && extent == extent;
        }
    };

    class AsyncData
    {
    public:

    private:

    };
}