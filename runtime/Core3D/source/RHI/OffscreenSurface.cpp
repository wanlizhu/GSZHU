#include "Wanlix/Core3D/RHI/OffscreenSurface.h"

namespace Wanlix
{
    OffscreenSurface::Ptr OffscreenSurface::Create(
        int width,
        int height
    ) 
    {
        return std::shared_ptr<OffscreenSurface>(new OffscreenSurface(width, height));
    }

    OffscreenSurface::OffscreenSurface(
        int width,
        int height
    )
        : mTarget(width, height)
    {}

    OffscreenSurface::~OffscreenSurface() 
    {}

    SurfaceType OffscreenSurface::GetSurfaceType() const
    {
        return SurfaceType::Offscreen;
    }

    void* OffscreenSurface::GetNativeHandle() const
    {
        return nullptr;
    }

    Extent OffscreenSurface::GetContentSize() const
    {
        return mTarget.GetSize();
    }

    bool OffscreenSurface::AdaptForVideoMode(const VideoModeDescriptor& videoModeDesc)
    {
        return mTarget.SetSize(videoModeDesc.resolution);
    }

    IDisplay::UniquePtr OffscreenSurface::GetResidentDisplay() const
    {
        return nullptr;
    }
}