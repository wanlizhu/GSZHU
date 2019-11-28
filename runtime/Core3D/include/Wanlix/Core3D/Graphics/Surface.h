#pragma once

#include "Wanlix/Core3D/Config.h"
#include "Wanlix/Core3D/Types.h"
#include "Wanlix/Core3D/Platform/Display.h"
#include "Wanlix/Core3D/Utility/NonCopyable.h"
#include "RenderContextDescriptor.h"

namespace Wanlix
{
    enum class SurfaceType
    {
        Offscreen,
        DesktopWindow,
        MobileCanvas,
    };

    // Surface provides the minimal required interface for a graphics rendering context,
    // such as the access to the native handle, information about the content size
    // and the ability to adapt for a new video mode or an updated pixel format.
    class Surface : public NonCopyable
    {
    public:
        using Ptr = std::shared_ptr<Surface>;
        using ConstPtr = std::shared_ptr<const Surface>;

        virtual SurfaceType GetSurfaceType() const = 0;
        virtual void* GetNativeHandle() const = 0;
        virtual Extent GetContentSize() const = 0;
        virtual bool AdaptForVideoMode(const VideoModeDescriptor& videoModeDesc) = 0;
        virtual Display::UniquePtr GetResidentDisplay() const = 0;
    };
}