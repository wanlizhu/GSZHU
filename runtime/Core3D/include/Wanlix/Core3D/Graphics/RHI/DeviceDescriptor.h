#pragma once

#include "Wanlix/Core3D/Graphics/Types.h"

namespace Wanlix
{
    struct VsyncDescriptor
    {
        bool     enabled = false;
        uint32_t refreshRate = 60;
        uint32_t interval = 1;
    };

    struct VideoModeDescriptor
    {
        Extent   resolution;
        int      colorBits = 32;
        int      depthBits = 24;
        int      stencilBits = 8;
        bool     fullscreen = false;
        uint32_t swapChainSize = 2;
    };

    struct RenderContextDescriptor
    {
        VsyncDescriptor     vsync;
        uint32_t            samples = 1;
        VideoModeDescriptor videoMode;
    };

}