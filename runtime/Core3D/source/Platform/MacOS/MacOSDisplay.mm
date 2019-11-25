#include "Wanlix/Platform/MacOS/MacOSDisplay.h"

namespace Wanlix
{
    // Converts a CGDisplayMode to a descriptor structure
    static void Convert(
        DisplayModeDescriptor& dst,
        CGDisplayModeRef src
    )
    {
        dst.resolution.width    = static_cast<std::uint32_t>(CGDisplayModeGetWidth(src));
        dst.resolution.height   = static_cast<std::uint32_t>(CGDisplayModeGetHeight(src));
        dst.refreshRate         = static_cast<std::uint32_t>(CGDisplayModeGetRefreshRate(src));
    }

    // Returns true if the specified descriptor matches the display mode
    static bool MatchDisplayMode(
        const DisplayModeDescriptor& displayModeDesc, 
        CGDisplayModeRef modeRef
    )
    {
        return
        (
            static_cast<std::size_t>(displayModeDesc.resolution.width ) == CGDisplayModeGetWidth (modeRef) &&
            static_cast<std::size_t>(displayModeDesc.resolution.height) == CGDisplayModeGetHeight(modeRef)
        );
    }
}