#include "GIPipelineDynamicStateVk.h"
#include "GIPipelineVk.h"
#include "GIDeviceVk.h"

namespace AutoCAD::Graphics::Engine
{
    GIViewportStateVk::GIViewportStateVk(const VkViewport& viewport)
        : mViewport(viewport)
    {}

    VkDynamicState GIViewportStateVk::GetType() const
    {
        return VK_DYNAMIC_STATE_VIEWPORT;
    }

    bool GIViewportStateVk::IsPendingUpdate() const
    {
        return mIsPendingUpdate;
    }


        const VkViewport& GIViewportStateVk::GetViewport() const;


        GIScissorStateVk::GIScissorStateVk(const VkRect2D& scissor);

        VkDynamicState GIScissorStateVk::GetType() const override final;
        bool GIScissorStateVk::IsPendingUpdate() const override final;

        const VkRect2D& GIScissorStateVk::GetScissor() const;

        GILineWidthStateVk::GILineWidthStateVk(float width);
      
        VkDynamicState GILineWidthStateVk::GetType() const override final;
        bool GILineWidthStateVk::IsPendingUpdate() const override final;

        float GILineWidthStateVk::GetLineWidth() const;


    class GIDepthBiasStateVk : public GIPipelineDynamicStateVk
    {
    public:
        GIDepthBiasStateVk(float constantFactor, float clamp, float slopeFactor);
        virtual ~GIDepthBiasStateVk() = default;
        virtual VkDynamicState GetType() const override final;
        virtual bool IsPendingUpdate() const override final;

        float GetConstantFactor() const;
        float GetClamp() const;
        float GetSlopeFactor() const;

    private:
        float mConstantFactor = 0.0f;
        float mClamp = 0.0f;
        float mSlopeFactor = 0.0f;
    };

    class GIBlendConstantsStateVk : public GIPipelineDynamicStateVk
    {
    public:
        GIBlendConstantsStateVk(const float* constants);
        virtual ~GIBlendConstantsStateVk() = default;
        virtual VkDynamicState GetType() const override final;
        virtual bool IsPendingUpdate() const override final;

        const float* GetConstants() const;

    private:
        float mConstants[4];
    };

    class GIDepthBoundsStateVk : public GIPipelineDynamicStateVk
    {
    public:
        GIDepthBoundsStateVk(float minBound, float maxBound);
        virtual ~GIDepthBoundsStateVk() = default;
        virtual VkDynamicState GetType() const override final;
        virtual bool IsPendingUpdate() const override final;

        float GetMinBound() const;
        float GetMaxBound() const;

    private:
        float mMinBound = 0.0f;
        float mMaxBound = 1.0f;
    };

    class GIStencilCompareMaskStateVk : public GIPipelineDynamicStateVk
    {
    public:
        GIStencilCompareMaskStateVk(VkStencilFaceFlagBits face, uint32_t mask);
        virtual ~GIStencilCompareMaskStateVk() = default;
        virtual VkDynamicState GetType() const override final;
        virtual bool IsPendingUpdate() const override final;

        VkStencilFaceFlagBits GetStencilFace() const;
        uint32_t GetCompareMask() const;

    private:
        VkStencilFaceFlagBits mStencilFace;
        uint32_t mCompareMask = 0;
    };

    class GIStencilWriteMaskStateVk : public GIPipelineDynamicStateVk
    {
    public:
        GIStencilWriteMaskStateVk(VkStencilFaceFlagBits face, uint32_t mask);
        virtual ~GIStencilWriteMaskStateVk() = default;
        virtual VkDynamicState GetType() const override final;
        virtual bool IsPendingUpdate() const override final;

        VkStencilFaceFlagBits GetStencilFace() const;
        uint32_t GetWriteMask() const;

    private:
        VkStencilFaceFlagBits mStencilFace;
        uint32_t mWriteMask = 0;
    };

    class GIStencilReferenceStateVk : public GIPipelineDynamicStateVk
    {
    public:
        GIStencilReferenceStateVk(VkStencilFaceFlagBits face, uint32_t reference);
        virtual ~GIStencilReferenceStateVk() = default;
        virtual VkDynamicState GetType() const override final;
        virtual bool IsPendingUpdate() const override final;

        VkStencilFaceFlagBits GetStencilFace() const;
        uint32_t GetReference() const;

    private:
        VkStencilFaceFlagBits mStencilFace;
        uint32_t mReference = 0;
    };
}