#include "GIDynamicDepthBiasVk.h"
#include "GIDeviceVk.h"

namespace AutoCAD::Graphics::Engine
{
    GIDynamicDepthBiasVk::GIDynamicDepthBiasVk(float constantFactor, float clamp, float slopeFactor)
        : mConstantFactor(constantFactor)
        , mClamp(clamp)
        , mSlopeFactor(slopeFactor)
    {}

    GIDynamicDepthBiasVk::GIDynamicDepthBiasVk(const GIDynamicDepthBiasVk& rhs)
        : mConstantFactor(rhs.mConstantFactor)
        , mClamp(rhs.mClamp)
        , mSlopeFactor(rhs.mSlopeFactor)
        , mIsPendingUpdate(rhs.mIsPendingUpdate)
    {}

    GIDynamicDepthBiasVk::GIDynamicDepthBiasVk(GIDynamicDepthBiasVk&& rhs)
        : mConstantFactor(rhs.mConstantFactor)
        , mClamp(rhs.mClamp)
        , mSlopeFactor(rhs.mSlopeFactor)
        , mIsPendingUpdate(rhs.mIsPendingUpdate)
    {}

    GIDynamicDepthBiasVk& GIDynamicDepthBiasVk::operator=(const GIDynamicDepthBiasVk& rhs)
    {
        mIsPendingUpdate = !(*this == rhs);
        mConstantFactor = rhs.mConstantFactor;
        mClamp = rhs.mClamp;
        mSlopeFactor = rhs.mSlopeFactor;
        return *this;
    }

    GIDynamicDepthBiasVk& GIDynamicDepthBiasVk::operator=(GIDynamicDepthBiasVk&& rhs)
    {
        mIsPendingUpdate = !(*this == rhs);
        mConstantFactor = rhs.mConstantFactor;
        mClamp = rhs.mClamp;
        mSlopeFactor = rhs.mSlopeFactor;
        return *this;
    }

    GIDynamicDepthBiasVk::~GIDynamicDepthBiasVk()
    {}

    VkDynamicState GIDynamicDepthBiasVk::GetType() const
    {
        return VK_DYNAMIC_STATE_DEPTH_BIAS;
    }

    bool GIDynamicDepthBiasVk::IsPendingUpdate() const
    {
        return mIsPendingUpdate;
    }

    bool GIDynamicDepthBiasVk::operator==(const GIDynamicDepthBiasVk& rhs) const
    {
        return std::abs(mConstantFactor - rhs.mConstantFactor) < std::numeric_limits<float>::epsilon()
            && std::abs(mClamp - rhs.mClamp) < std::numeric_limits<float>::epsilon()
            && std::abs(mSlopeFactor - rhs.mSlopeFactor) < std::numeric_limits<float>::epsilon();
    }

    void GIDynamicDepthBiasVk::SetConstantFactor(float constantFactor)
    {
        mConstantFactor = constantFactor;
    }

    void GIDynamicDepthBiasVk::SetClamp(float clamp)
    {
        mClamp = clamp;
    }

    void GIDynamicDepthBiasVk::SetSlopeFactor(float slopeFactor)
    {
        mSlopeFactor = slopeFactor;
    }

    float GIDynamicDepthBiasVk::GetConstantFactor() const
    {
        return mConstantFactor;
    }

    float GIDynamicDepthBiasVk::GetClamp() const
    {
        return mClamp;
    }

    float GIDynamicDepthBiasVk::GetSlopeFactor() const
    {
        return mSlopeFactor;
    }
}