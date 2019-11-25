#include "Wanlix/Platform/DisplayModeDescriptor.h"

namespace Wanlix
{
    bool DisplayModeDescriptor::operator<(const DisplayModeDescriptor& mode) const
    {
        const auto lhsNumPixels = resolution.width * resolution.height;
        const auto rhsNumPixels = mode.resolution.width * mode.resolution.height;

        if (lhsNumPixels < rhsNumPixels)
            return true;
        if (lhsNumPixels > rhsNumPixels)
            return false;

        return (refreshRate < mode.refreshRate);
    }

    bool DisplayModeDescriptor::operator==(const DisplayModeDescriptor& mode) const
    {
        return resolution == mode.resolution
            && refreshRate == mode.refreshRate;
    }

    bool DisplayModeDescriptor::operator!=(const DisplayModeDescriptor& mode) const
    {
        return !(*this == mode);
    }

    // Computes the greatest common divisor (GCD) for the two parameters
    static std::uint32_t ComputeGCD(std::uint32_t a, std::uint32_t b)
    {
        while (b != 0)
        {
            auto r = a % b;
            a = b;
            b = r;
        }
        return a;
    }

    Extent DisplayModeDescriptor::GetExtentRatio() const
    {
        auto gcd = ComputeGCD(resolution.width, resolution.height);
        return { resolution.width / gcd, resolution.height / gcd };
    }
}