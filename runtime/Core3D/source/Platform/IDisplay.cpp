#include "Wanlix/Core3D/Platform/Display.h"
#include <algorithm>

namespace Wanlix
{
    void Display::FinalizeDisplayModes(std::vector<DisplayModeDescriptor>& modes)
    {
        // Sort display mode descriptors in ascending order (with, height, frequency) 
        std::sort(modes.begin(), modes.end());

        // Remove all duplicates
        modes.erase(std::unique(modes.begin(), modes.end()), modes.end());
    }
}