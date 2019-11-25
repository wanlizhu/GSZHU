#pragma once

#include "Wanlix/Core/EngineConfig.h"
#include "Wanlix/Core/Types.h"
#include "RenderContextDescriptor.h"
#include "Wanlix/Platform/IDisplay.h"
#include "Wanlix/Utility/NonCopyable.h"

namespace Wanlix
{
    // Surface provides the minimal required interface for a graphics rendering context,
    // such as the access to the native handle, information about the content size
    // and the ability to adapt for a new video mode or an updated pixel format.
    class ISurface : public NonCopyable
    {
    public:
        using Ptr = std::shared_ptr<ISurface>;
        using ConstPtr = std::shared_ptr<const ISurface>;

        // Returns the native surface handle.
        virtual bool GetNativeHandle(
            void* handle,
            size_t handleSize
        ) const = 0;

        // Returns the size of the surface context (or rather the drawing area).
        virtual Extent GetSize() const = 0;

        // Adapts the surface to fits the needs for the specified video mode descriptor.
        virtual bool AdaptForVideoMode(const VideoModeDescriptor& videoModeDesc) = 0;

        // Resets the internal pixel format of the surface.
        // This function is mainly used on Win32 when a multi-sampled framebuffer is created.
        virtual void ResetPixelFormat() = 0;

        // Processes all events for this surface, i.e. input-, movement-, resize-, and other events.
        virtual bool ProcessEvents() = 0;

        // Instantiates the Display interface where this surface is resident in.
        virtual IDisplay::UniquePtr GetResidentDisplay() const = 0;
    };
}