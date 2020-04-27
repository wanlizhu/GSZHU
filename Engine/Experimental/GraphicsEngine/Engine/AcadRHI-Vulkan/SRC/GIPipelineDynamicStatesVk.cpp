#include "GIPipelineDynamicStatesVk.h"
#include "GIDeviceVk.h"

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
        : mViewport(rhs.mViewport)
        , mIsPendingUpdate(rhs.mIsPendingUpdate)
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

    GIDynamicScissorVk::GIDynamicScissorVk(const VkRect2D& scissor)
        : mScissor(scissor)
        , mIsPendingUpdate(true)
    {}

    GIDynamicScissorVk::GIDynamicScissorVk(const GIDynamicScissorVk& rhs)
        : mScissor(rhs.mScissor)
        , mIsPendingUpdate(rhs.mIsPendingUpdate)
    {}

    GIDynamicScissorVk::GIDynamicScissorVk(GIDynamicScissorVk&& rhs)
        : mScissor(rhs.mScissor)
        , mIsPendingUpdate(rhs.mIsPendingUpdate)
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

    GIDynamicLineWidthVk::GIDynamicLineWidthVk(float width)
        : mLineWidth(width)
        , mIsPendingUpdate(false)
    {}

    GIDynamicLineWidthVk::GIDynamicLineWidthVk(const GIDynamicLineWidthVk& rhs)
        : mLineWidth(rhs.mLineWidth)
        , mIsPendingUpdate(rhs.mIsPendingUpdate)
    {}

    GIDynamicLineWidthVk::GIDynamicLineWidthVk(GIDynamicLineWidthVk&& rhs)
        : mLineWidth(rhs.mLineWidth)
        , mIsPendingUpdate(rhs.mIsPendingUpdate)
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

    bool GIDynamicLineWidthVk::operator==(const GIDynamicLineWidthVk& rhs) const
    {
        return std::abs(mLineWidth - rhs.mLineWidth) < std::numeric_limits<float>::epsilon();
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

    bool GIDynamicBlendConstantsVk::operator==(const GIDynamicBlendConstantsVk& rhs) const
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

    GIDynamicDepthBoundsVk::GIDynamicDepthBoundsVk(float minBound, float maxBound)
        : mMinBound(minBound)
        , mMaxBound(maxBound)
        , mIsPendingUpdate(true)
    {}

    GIDynamicDepthBoundsVk::GIDynamicDepthBoundsVk(const GIDynamicDepthBoundsVk& rhs)
        : mMinBound(rhs.mMinBound)
        , mMaxBound(rhs.mMaxBound)
        , mIsPendingUpdate(rhs.mIsPendingUpdate)
    {}

    GIDynamicDepthBoundsVk::GIDynamicDepthBoundsVk(GIDynamicDepthBoundsVk&& rhs)
        : mMinBound(rhs.mMinBound)
        , mMaxBound(rhs.mMaxBound)
        , mIsPendingUpdate(rhs.mIsPendingUpdate)
    {}

    GIDynamicDepthBoundsVk& GIDynamicDepthBoundsVk::operator=(const GIDynamicDepthBoundsVk& rhs)
    {
        mIsPendingUpdate = !(*this == rhs);
        mMinBound = rhs.mMinBound;
        mMaxBound = rhs.mMaxBound;
        return *this;
    }

    GIDynamicDepthBoundsVk& GIDynamicDepthBoundsVk::operator=(GIDynamicDepthBoundsVk&& rhs)
    {
        mIsPendingUpdate = !(*this == rhs);
        mMinBound = rhs.mMinBound;
        mMaxBound = rhs.mMaxBound;
        return *this;
    }

    GIDynamicDepthBoundsVk::~GIDynamicDepthBoundsVk()
    {}

    VkDynamicState GIDynamicDepthBoundsVk::GetType() const
    {
        return VK_DYNAMIC_STATE_DEPTH_BOUNDS;
    }

    bool GIDynamicDepthBoundsVk::IsPendingUpdate() const
    {
        return mIsPendingUpdate;
    }

    bool GIDynamicDepthBoundsVk::operator==(const GIDynamicDepthBoundsVk& rhs) const
    {
        return std::abs(mMinBound - rhs.mMinBound) < std::numeric_limits<float>::epsilon()
            && std::abs(mMaxBound - rhs.mMaxBound) < std::numeric_limits<float>::epsilon();
    }

    void GIDynamicDepthBoundsVk::SetMinBound(float minBound)
    {
        mIsPendingUpdate = std::abs(mMinBound - minBound) < std::numeric_limits<float>::epsilon();
        mMinBound = minBound;
    }

    void GIDynamicDepthBoundsVk::SetMaxBound(float maxBound)
    {
        mIsPendingUpdate = std::abs(mMaxBound - maxBound) < std::numeric_limits<float>::epsilon();
        mMaxBound = maxBound;
    }

    float GIDynamicDepthBoundsVk::GetMinBound() const
    {
        return mMinBound;
    }

    float GIDynamicDepthBoundsVk::GetMaxBound() const
    {
        return mMaxBound;
    }

    GIDynamicStencilCompareMaskVk::GIDynamicStencilCompareMaskVk(VkStencilFaceFlagBits stencilFace, uint32_t compareMask)
        : mStencilFace(stencilFace)
        , mCompareMask(compareMask)
        , mIsPendingUpdate(true)
    {}

    GIDynamicStencilCompareMaskVk::GIDynamicStencilCompareMaskVk(const GIDynamicStencilCompareMaskVk& rhs)
        : mStencilFace(rhs.mStencilFace)
        , mCompareMask(rhs.mCompareMask)
        , mIsPendingUpdate(rhs.mIsPendingUpdate)
    {}

    GIDynamicStencilCompareMaskVk::GIDynamicStencilCompareMaskVk(GIDynamicStencilCompareMaskVk&& rhs)
        : mStencilFace(rhs.mStencilFace)
        , mCompareMask(rhs.mCompareMask)
        , mIsPendingUpdate(rhs.mIsPendingUpdate)
    {}

    GIDynamicStencilCompareMaskVk& GIDynamicStencilCompareMaskVk::operator=(const GIDynamicStencilCompareMaskVk& rhs)
    {
        mIsPendingUpdate = !(*this == rhs);
        mStencilFace = rhs.mStencilFace;
        mCompareMask = rhs.mCompareMask;
        return *this;
    }

    GIDynamicStencilCompareMaskVk& GIDynamicStencilCompareMaskVk::operator=(GIDynamicStencilCompareMaskVk&& rhs)
    {
        mIsPendingUpdate = !(*this == rhs);
        mStencilFace = rhs.mStencilFace;
        mCompareMask = rhs.mCompareMask;
        return *this;
    }

    GIDynamicStencilCompareMaskVk::~GIDynamicStencilCompareMaskVk()
    {}

    VkDynamicState GIDynamicStencilCompareMaskVk::GetType() const
    {
        return VK_DYNAMIC_STATE_STENCIL_COMPARE_MASK;
    }

    bool GIDynamicStencilCompareMaskVk::IsPendingUpdate() const
    {
        return mIsPendingUpdate;
    }

    bool GIDynamicStencilCompareMaskVk::operator==(const GIDynamicStencilCompareMaskVk& rhs) const
    {
        return mStencilFace == rhs.mStencilFace
            && mCompareMask == rhs.mCompareMask;
    }

    void GIDynamicStencilCompareMaskVk::SetStencilFace(VkStencilFaceFlagBits stencilFace)
    {
        mStencilFace = stencilFace;
    }

    void GIDynamicStencilCompareMaskVk::SetCompareMask(uint32_t compareMask)
    {
        mCompareMask = compareMask;
    }

    VkStencilFaceFlagBits GIDynamicStencilCompareMaskVk::GetStencilFace() const
    {
        return mStencilFace;
    }

    uint32_t GIDynamicStencilCompareMaskVk::GetCompareMask() const
    {
        return mCompareMask;
    }

    GIDynamicStencilWriteMaskVk::GIDynamicStencilWriteMaskVk(VkStencilFaceFlagBits stencilFace, uint32_t writeMask)
        : mStencilFace(stencilFace)
        , mWriteMask(writeMask)
        , mIsPendingUpdate(true)
    {}

    GIDynamicStencilWriteMaskVk::GIDynamicStencilWriteMaskVk(const GIDynamicStencilWriteMaskVk& rhs)
        : mStencilFace(rhs.mStencilFace)
        , mWriteMask(rhs.mWriteMask)
        , mIsPendingUpdate(rhs.mIsPendingUpdate)
    {}

    GIDynamicStencilWriteMaskVk::GIDynamicStencilWriteMaskVk(GIDynamicStencilWriteMaskVk&& rhs)
        : mStencilFace(rhs.mStencilFace)
        , mWriteMask(rhs.mWriteMask)
        , mIsPendingUpdate(rhs.mIsPendingUpdate)
    {}

    GIDynamicStencilWriteMaskVk& GIDynamicStencilWriteMaskVk::operator=(const GIDynamicStencilWriteMaskVk& rhs)
    {
        mIsPendingUpdate = !(*this == rhs);
        mStencilFace = rhs.mStencilFace;
        mWriteMask = rhs.mWriteMask;
        return *this;
    }

    GIDynamicStencilWriteMaskVk& GIDynamicStencilWriteMaskVk::operator=(GIDynamicStencilWriteMaskVk&& rhs)
    {
        mIsPendingUpdate = !(*this == rhs);
        mStencilFace = rhs.mStencilFace;
        mWriteMask = rhs.mWriteMask;
        return *this;
    }

    GIDynamicStencilWriteMaskVk::~GIDynamicStencilWriteMaskVk()
    {}

    VkDynamicState GIDynamicStencilWriteMaskVk::GetType() const
    {
        return VK_DYNAMIC_STATE_STENCIL_WRITE_MASK;
    }

    bool GIDynamicStencilWriteMaskVk::IsPendingUpdate() const
    {
        return mIsPendingUpdate;
    }

    bool GIDynamicStencilWriteMaskVk::operator==(const GIDynamicStencilWriteMaskVk& rhs) const
    {
        return mStencilFace == rhs.mStencilFace
            && mWriteMask == rhs.mWriteMask;
    }

    void GIDynamicStencilWriteMaskVk::SetStencilFace(VkStencilFaceFlagBits stencilFace)
    {
        mStencilFace = stencilFace;
    }

    void GIDynamicStencilWriteMaskVk::SetWriteMask(uint32_t writeMask)
    {
        mWriteMask = writeMask;
    }

    VkStencilFaceFlagBits GIDynamicStencilWriteMaskVk::GetStencilFace() const
    {
        return mStencilFace;
    }

    uint32_t GIDynamicStencilWriteMaskVk::GetWriteMask() const
    {
        return mWriteMask;
    }
}