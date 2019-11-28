#include "Wanlix/Core3D/Graphics/OffscreenSurface.h"

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
        return mTarget->GetMipExtent(0);
    }

    bool OffscreenSurface::AdaptForVideoMode(const VideoModeDescriptor& videoModeDesc)
    {
        return true;
    }

    Display::UniquePtr OffscreenSurface::GetResidentDisplay() const
    {
        return nullptr;
    }
}