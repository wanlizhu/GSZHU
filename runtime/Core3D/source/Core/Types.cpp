#include "Wanlix/Core/Types.h"
#include <limits>

namespace Wanlix
{
    template<typename T>
    static T AddClamped(const T& a, const T& b)
    {
        static const tMax = std::numeric_limits<T>::max();
        uint64_t x = a;
        x += b;

        if (x > static_cast<std::uint64_t>(tMax))
            return tMax;
        else
            return a + b;
    }

    Range Range::operator+(uint32_t count) const
    {
        return Range(begin, size + count);
    }

    Range Range::operator-(uint32_t count) const
    {
        return Range(begin, size - count);
    }

    bool Range::operator==(const Range& other) const
    {
        return begin == other.begin && size == other.size;
    }

    bool Range::operator!=(const Range& other) const
    {
        return !(*this == other);
    }

    Extent Extent::operator+(const Extent& other) const
    {
        return Extent(width + other.width,
                      height + other.height,
                      depth + other.depth);
    }

    Extent Extent::operator-(const Extent& other) const
    {
        return Extent(width - other.width,
                      height - other.height,
                      depth - other.depth);
    }

    bool Extent::operator==(const Extent& other) const
    {
        return width == other.width
            && height == other.height
            && depth == other.depth;
    }

    bool Extent::operator!=(const Extent& other) const
    {
        return !(*this == other);
    }

    Offset Offset::operator+(const Offset& other) const
    {
        return Offset(x + other.x,
                      y + other.y,
                      z + other.z);
    }

    Offset Offset::operator-(const Offset& other) const
    {
        return Offset(x - other.x,
                      y - other.y,
                      z - other.z);
    }

    bool Offset::operator==(const Offset& other) const
    {
        return x == other.x
            && y == other.y
            && z == other.z;
    }

    bool Offset::operator!=(const Offset& other) const
    {
        return !(*this == other);
    }

    Region Region::operator+(const Extent& ext) const
    {
        return Region(offset, extent + ext);
    }

    Region Region::operator-(const Extent& ext) const
    {
        return Region(offset, extent - ext);
    }

    bool Region::operator==(const Region& other) const
    {
        return offset == other.offset && extent == other.extent;
    }

    bool Region::operator!=(const Region& other) const
    {
        return !(*this == other);
    }
}