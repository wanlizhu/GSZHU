#include "GIDynamicLineWidthVk.h"
#include "GIDeviceVk.h"

namespace AutoCAD::Graphics::Engine
{
    GIDynamicLineWidthVk::GIDynamicLineWidthVk(float width)
        : mLineWidth(width)
        , mIsPendingUpdate(false)
    {}

    GIDynamicLineWidthVk::GIDynamicLineWidthVk(const GIDynamicLineWidthVk& rhs)
        : mLineWidth(rhs.mLineWidth)
        , mIsPendingUpdate(rhs.mIsPendingUpdate)
    {}

    GIDynamicLineWidthVk::GIDynamicLineWidthVk(GIDynamicLineWidthVk&& rhs)
        : mLineWidth(std::move(rhs.mLineWidth))
        , mIsPendingUpdate(std::move(rhs.mIsPendingUpdate))
    {}

    GIDynamicLineWidthVk& GIDynamicLineWidthVk::operator=(const GIDynamicLineWidthVk& rhs)
    {
        SetLineWidth(rhs.mLineWidth);
        return *this;
    }

    GIDynamicLineWidthVk& GIDynamicLineWidthVk::operator=(GIDynamicLineWidthVk&& rhs)
    {
        SetLineWidth(rhs.mLineWidth);
        return *this;
    }

    GIDynamicLineWidthVk::~GIDynamicLineWidthVk()
    {}

    VkDynamicState GIDynamicLineWidthVk::GetType() const
    {
        return VK_DYNAMIC_STATE_LINE_WIDTH;
    }

    bool GIDynamicLineWidthVk::IsPendingUpdate() const
    {
        return mIsPendingUpdate;
    }

    void GIDynamicLineWidthVk::SetLineWidth(float width)
    {
        mIsPendingUpdate = std::abs(mLineWidth - width) > std::numeric_limits<float>::epsilon();
        mLineWidth = width;
    }

    float GIDynamicLineWidthVk::GetLineWidth() const
    {
        return mLineWidth;
    }
}