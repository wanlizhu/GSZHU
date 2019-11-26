#include "Wanlix/Core3D/RHI/RenderContextDescriptor.h"

namespace Wanlix
{
    bool VsyncDescriptor::operator==(const VsyncDescriptor& other) const
    {
        return enabled == other.enabled
            && refreshRate == other.refreshRate
            && interval == other.interval;
    }

    bool VsyncDescriptor::operator!=(const VsyncDescriptor& other) const
    {
        return !(*this == other);
    }

    bool VideoModeDescriptor::operator==(const VideoModeDescriptor& other) const
    {
        return resolution == other.resolution
            && colorBits == other.colorBits
            && depthBits == other.depthBits
            && stencilBits == other.stencilBits
            && fullscreen == other.fullscreen
            && swapChainSize == other.swapChainSize;
    }

    bool VideoModeDescriptor::operator!=(const VideoModeDescriptor& other) const
    {
        return !(*this == other);
    }

    bool RenderContextDescriptor::operator==(const RenderContextDescriptor& other) const
    {
        return vsync == other.vsync
            && videoMode == other.videoMode
            && samples == other.samples;
    }

    bool RenderContextDescriptor::operator!=(const RenderContextDescriptor& other) const
    {
        return !(*this == other);
    }
}