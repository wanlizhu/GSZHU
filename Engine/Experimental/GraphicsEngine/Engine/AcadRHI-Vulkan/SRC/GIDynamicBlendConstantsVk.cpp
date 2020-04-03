#include "GIDynamicBlendConstantsVk.h"
#include "GIDeviceVk.h"

namespace AutoCAD::Graphics::Engine
{
    GIDynamicBlendConstantsVk::GIDynamicBlendConstantsVk(const float* constants)
        : mIsPendingUpdate(false)
    {
        std::memcpy(mConstants, constants, sizeof(float) * 4);
    }

    GIDynamicBlendConstantsVk::GIDynamicBlendConstantsVk(const GIDynamicBlendConstantsVk& rhs)
        : mIsPendingUpdate(rhs.mIsPendingUpdate)
    {
        std::memcpy(mConstants, rhs.mConstants, sizeof(float) * 4);
    }

    GIDynamicBlendConstantsVk::GIDynamicBlendConstantsVk(GIDynamicBlendConstantsVk&& rhs)
        : mIsPendingUpdate(rhs.mIsPendingUpdate)
    {
        std::memcpy(mConstants, rhs.mConstants, sizeof(float) * 4);
    }

    GIDynamicBlendConstantsVk& GIDynamicBlendConstantsVk::operator=(const GIDynamicBlendConstantsVk& rhs)
    {
        SetConstants(rhs.mConstants);
        return *this;
    }

    GIDynamicBlendConstantsVk& GIDynamicBlendConstantsVk::operator=(GIDynamicBlendConstantsVk&& rhs)
    {
        SetConstants(rhs.mConstants);
        return *this;
    }

    GIDynamicBlendConstantsVk::~GIDynamicBlendConstantsVk()
    {}

    VkDynamicState GIDynamicBlendConstantsVk::GetType() const
    {
        return VK_DYNAMIC_STATE_BLEND_CONSTANTS;
    }

    bool GIDynamicBlendConstantsVk::IsPendingUpdate() const
    {
        return mIsPendingUpdate;
    }

    bool GIDynamicBlendConstantsVk::operator==(const GIDynamicBlendConstantsVk & rhs) const
    {
        for (int i = 0; i < 4; i++)
        {
            if (std::abs(mConstants[i] - rhs.mConstants[i]) > std::numeric_limits<float>::epsilon())
            {
                return false;
            }
        }
        return true;
    }

    void GIDynamicBlendConstantsVk::SetConstants(const float* constants)
    {
        mIsPendingUpdate = false;
        for (int i = 0; i < 4; i++)
        {
            if (std::abs(mConstants[i] - constants[i]) > std::numeric_limits<float>::epsilon())
            {
                mIsPendingUpdate = true;
                break;
            }
        }

        std::memcpy(mConstants, constants, sizeof(float) * 4);
    }

    const float* GIDynamicBlendConstantsVk::GetConstants() const
    {
        return mConstants;
    }
}