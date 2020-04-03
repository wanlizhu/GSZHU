#include "GIDynamicScissorVk.h"

namespace AutoCAD::Graphics::Engine
{
    GIDynamicScissorVk::GIDynamicScissorVk(const VkRect2D& scissor)
        : mScissor(scissor)
        , mIsPendingUpdate(true)
    {}

    GIDynamicScissorVk::GIDynamicScissorVk(const GIDynamicScissorVk& rhs)
        : mScissor(rhs.mScissor)
        , mIsPendingUpdate(rhs.mIsPendingUpdate)
    {}

    GIDynamicScissorVk::GIDynamicScissorVk(GIDynamicScissorVk&& rhs)
        : mScissor(std::move(rhs.mScissor))
        , mIsPendingUpdate(std::move(rhs.mIsPendingUpdate))
    {}

    GIDynamicScissorVk& GIDynamicScissorVk::operator=(const GIDynamicScissorVk& rhs)
    {
        SetScissor(rhs.mScissor);
        return *this;
    }

    GIDynamicScissorVk& GIDynamicScissorVk::operator=(GIDynamicScissorVk&& rhs)
    {
        SetScissor(rhs.mScissor);
        return *this;
    }

    GIDynamicScissorVk::~GIDynamicScissorVk()
    {}

    VkDynamicState GIDynamicScissorVk::GetType() const
    {
        return VK_DYNAMIC_STATE_SCISSOR;
    }

    bool GIDynamicScissorVk::IsPendingUpdate() const
    {
        return mIsPendingUpdate;
    }

    bool GIDynamicScissorVk::operator==(const GIDynamicScissorVk& rhs) const
    {
        return mScissor == rhs.mScissor;
    }

    void GIDynamicScissorVk::SetScissor(const VkRect2D& scissor)
    {
        mIsPendingUpdate = !(mScissor == scissor);
        mScissor = scissor;
    }

    const VkRect2D& GIDynamicScissorVk::GetScissor() const
    {
        return mScissor;
    }
}