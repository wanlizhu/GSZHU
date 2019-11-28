#pragma once

#include "Wanlix/Core3D/Types.h"
#include <cstdint>

namespace Wanlix
{
    struct VsyncDescriptor
    {
        bool enabled = false;
        uint32_t refreshRate = 60; // Only supported with: Direct3D 11, Direct3D 12, Metal.
        uint32_t interval = 1; // Synchronisation interval. Can be 1, 2, 3, or 4. By default 1.
        
        bool operator==(const VsyncDescriptor& other) const;
        bool operator!=(const VsyncDescriptor& other) const;
    };

    // This is mainly used to set the video mode of a RenderContext object.
    // The counterpart for a physical display mode is the DisplayModeDescriptor structure.
    struct VideoModeDescriptor
    {
        Extent resolution;
        int colorBits = 32;
        int depthBits = 24;
        int stencilBits = 8;
        bool fullscreen = false;
        uint32_t swapChainSize = 2;

        bool operator==(const VideoModeDescriptor& other) const;
        bool operator!=(const VideoModeDescriptor& other) const;
    };

    struct RenderContextDescriptor
    {
        VideoModeDescriptor videoMode;
        VsyncDescriptor vsync;
        uint32_t samples = 1;

        bool operator==(const RenderContextDescriptor& other) const;
        bool operator!=(const RenderContextDescriptor& other) const;
    };
}
