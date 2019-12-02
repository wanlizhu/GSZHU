#include "Wanlix/Core3D/Platform/Display.h"
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

    Extent DisplayModeDescriptor::GetExtentRatio() const
    {
        auto gcd = ComputeGCD(resolution.width, resolution.height);
        return { resolution.width / gcd, resolution.height / gcd };
    }

    bool DisplayModeDescriptor::operator==(const DisplayModeDescriptor& rhs) const
    {
        return refreshRate == rhs.refreshRate && resolution == rhs.resolution;
    }

    bool DisplayModeDescriptor::operator<(const DisplayModeDescriptor& rhs) const
    {
        const auto lhsNumPixels = resolution.width * resolution.height;
        const auto rhsNumPixels = rhs.resolution.width * rhs.resolution.height;

        if (lhsNumPixels < rhsNumPixels)
            return true;
        if (lhsNumPixels > rhsNumPixels)
            return false;

        return (refreshRate < rhs.refreshRate);
    }

    void Display::FinalizeDisplayModes(std::vector<DisplayModeDescriptor>& modes)
    {
        // Sort display mode descriptors in ascending order (with, height, frequency) 
        std::sort(modes.begin(), modes.end());

        // Remove all duplicates
        modes.erase(std::unique(modes.begin(), modes.end()), modes.end());
    }
}