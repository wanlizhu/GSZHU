#include "GIDynamicViewportStateVk.h"

namespace AutoCAD::Graphics::Engine
{
    static bool VkViewportEqual(const VkViewport& lhs, const VkViewport& rhs)
    {
        return lhs.x == rhs.x &&
            lhs.y == rhs.y && 
            lhs.width == rhs.width &&
            lhs.height == rhs.height &&
            lhs.minDepth == rhs.minDepth &&
            lhs.maxDepth == rhs.maxDepth;
    }

    GIDynamicViewportStateVk::GIDynamicViewportStateVk(const VkViewport& viewport)
        : mViewport(viewport)
        , mIsPendingUpdate(true)
    {}

    GIDynamicViewportStateVk::GIDynamicViewportStateVk(const GIDynamicViewportStateVk& rhs)
        : mViewport(rhs.mViewport)
        , mIsPendingUpdate(rhs.mIsPendingUpdate)
    {}

    GIDynamicViewportStateVk::GIDynamicViewportStateVk(GIDynamicViewportStateVk&& rhs)
        : mViewport(std::move(rhs.mViewport))
        , mIsPendingUpdate(std::move(rhs.mIsPendingUpdate))
    {}

    GIDynamicViewportStateVk& GIDynamicViewportStateVk::operator=(const GIDynamicViewportStateVk& rhs)
    {
        mIsPendingUpdate = !VkViewportEqual(mViewport, rhs.mViewport);
        mViewport = rhs.mViewport;
        return *this;
    }

    GIDynamicViewportStateVk& GIDynamicViewportStateVk::operator=(GIDynamicViewportStateVk&& rhs)
    {
        mIsPendingUpdate = !VkViewportEqual(mViewport, rhs.mViewport);
        mViewport = std::move(rhs.mViewport);
        return *this;
    }

    GIDynamicViewportStateVk::~GIDynamicViewportStateVk()
    {}

    VkDynamicState GIDynamicViewportStateVk::GetType() const
    {
        return VK_DYNAMIC_STATE_VIEWPORT;
    }

    bool GIDynamicViewportStateVk::IsPendingUpdate() const
    {
        return mIsPendingUpdate;
    }

    void GIDynamicViewportStateVk::SetViewport(const VkViewport& viewport)
    {
        mIsPendingUpdate = !VkViewportEqual(mViewport, viewport);
        mViewport = viewport;
    }

    const VkViewport& GIDynamicViewportStateVk::GetViewport() const
    {
        return mViewport;
    }
}