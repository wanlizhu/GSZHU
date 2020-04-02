#include "GIDynamicScissorStateVk.h"

namespace AutoCAD::Graphics::Engine
{
    static bool VkRect2DEqual(const VkRect2D& lhs, const VkRect2D& rhs)
    {
        return lhs.extent.width == rhs.extent.width &&
            lhs.extent.height == rhs.extent.height &&
            lhs.offset.x == rhs.offset.x &&
            lhs.offset.y == rhs.offset.y;
    }

    GIDynamicScissorStateVk::GIDynamicScissorStateVk(const VkRect2D& scissor)
        : mScissor(scissor)
        , mIsPendingUpdate(true)
    {}

    GIDynamicScissorStateVk::GIDynamicScissorStateVk(const GIDynamicScissorStateVk& rhs)
        : mScissor(rhs.mScissor)
        , mIsPendingUpdate(rhs.mIsPendingUpdate)
    {}

    GIDynamicScissorStateVk::GIDynamicScissorStateVk(GIDynamicScissorStateVk&& rhs)
        : mScissor(std::move(rhs.mScissor))
        , mIsPendingUpdate(std::move(rhs.mIsPendingUpdate))
    {}

    GIDynamicScissorStateVk& GIDynamicScissorStateVk::operator=(const GIDynamicScissorStateVk& rhs)
    {
        mIsPendingUpdate = !VkRect2DEqual(mScissor, rhs.mScissor);
        mScissor = rhs.mScissor;
        return *this;
    }

    GIDynamicScissorStateVk& GIDynamicScissorStateVk::operator=(GIDynamicScissorStateVk&& rhs)
    {
        mIsPendingUpdate = !VkRect2DEqual(mScissor, rhs.mScissor);
        mScissor = std::move(rhs.mScissor);
        return *this;
    }

    GIDynamicScissorStateVk::~GIDynamicScissorStateVk()
    {}

    VkDynamicState GIDynamicScissorStateVk::GetType() const
    {
        return VK_DYNAMIC_STATE_SCISSOR;
    }

    bool GIDynamicScissorStateVk::IsPendingUpdate() const
    {
        return mIsPendingUpdate;
    }

    void GIDynamicScissorStateVk::SetScissor(const VkRect2D& scissor)
    {
        mIsPendingUpdate = !VkRect2DEqual(mScissor, scissor);
        mScissor = scissor;
    }

    const VkRect2D& GIDynamicScissorStateVk::GetScissor() const
    {
        return mScissor;
    }
}