#include "GIDynamicViewportVk.h"

namespace AutoCAD::Graphics::Engine
{
    GIDynamicViewportVk::GIDynamicViewportVk(const VkViewport& viewport)
        : mViewport(viewport)
        , mIsPendingUpdate(true)
    {}

    GIDynamicViewportVk::GIDynamicViewportVk(const GIDynamicViewportVk& rhs)
        : mViewport(rhs.mViewport)
        , mIsPendingUpdate(rhs.mIsPendingUpdate)
    {}

    GIDynamicViewportVk::GIDynamicViewportVk(GIDynamicViewportVk&& rhs)
        : mViewport(std::move(rhs.mViewport))
        , mIsPendingUpdate(std::move(rhs.mIsPendingUpdate))
    {}

    GIDynamicViewportVk& GIDynamicViewportVk::operator=(const GIDynamicViewportVk& rhs)
    {
        SetViewport(rhs.mViewport);
        return *this;
    }

    GIDynamicViewportVk& GIDynamicViewportVk::operator=(GIDynamicViewportVk&& rhs)
    {
        SetViewport(rhs.mViewport);
        return *this;
    }

    GIDynamicViewportVk::~GIDynamicViewportVk()
    {}

    VkDynamicState GIDynamicViewportVk::GetType() const
    {
        return VK_DYNAMIC_STATE_VIEWPORT;
    }

    bool GIDynamicViewportVk::IsPendingUpdate() const
    {
        return mIsPendingUpdate;
    }

    bool GIDynamicViewportVk::operator==(const GIDynamicViewportVk& rhs) const
    {
        return mViewport == rhs.mViewport;
    }

    void GIDynamicViewportVk::SetViewport(const VkViewport& viewport)
    {
        mIsPendingUpdate = !(mViewport == viewport);
        mViewport = viewport;
    }

    const VkViewport& GIDynamicViewportVk::GetViewport() const
    {
        return mViewport;
    }
}