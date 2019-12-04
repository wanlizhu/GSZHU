#include "Wanlix/PAL/Display.h"
#include <algorithm>

namespace Wanlix
{
    // Computes the greatest common divisor (GCD) for the two parameters
    static uint32_t ComputeGCD(uint32_t a, uint32_t b)
    {
        while (b != 0)
        {
            auto r = a % b;
            a = b;
            b = r;
        }
        return a;
    }

    INT2 GetExtentRatio(const DisplayMode& mode)
    {
        auto gcd = (int)ComputeGCD(mode.resolution.width, mode.resolution.height);
        return INT2 { 
            mode.resolution.width / gcd, 
            mode.resolution.height / gcd 
        };
    }

    bool operator==(const DisplayMode& lhs, const DisplayMode& rhs)
    {
        return lhs.refreshRate == rhs.refreshRate && 
               lhs.resolution == rhs.resolution;
    }

    bool operator<(const DisplayMode& lhs, const DisplayMode& rhs)
    {
        const auto lhsNumPixels = lhs.resolution.width * lhs.resolution.height;
        const auto rhsNumPixels = rhs.resolution.width * rhs.resolution.height;

        if (lhsNumPixels < rhsNumPixels)
            return true;
        if (lhsNumPixels > rhsNumPixels)
            return false;

        return (lhs.refreshRate < rhs.refreshRate);
    }

    void Display::SortDisplayModes(std::vector<DisplayMode>& modes) const
    {
        // Sort display mode descriptors in ascending order (with, height, frequency) 
        std::sort(modes.begin(), modes.end());

        // Remove all duplicates
        modes.erase(std::unique(modes.begin(), modes.end()), modes.end());
    }
}