#pragma once

#include "GICommonVk.h"

namespace AutoCAD::Graphics::Engine
{
    class GIPipelineDynamicStateVk
    {
    public:
        virtual ~GIPipelineDynamicStateVk() = default;
        virtual VkDynamicState GetType() const = 0;
        virtual bool IsPendingUpdate() const = 0;
    };

    class GIDynamicViewportVk : public GIPipelineDynamicStateVk
    {
    public:
        GIDynamicViewportVk(const VkViewport& viewport);
        GIDynamicViewportVk(const GIDynamicViewportVk& rhs);
        GIDynamicViewportVk(GIDynamicViewportVk&& rhs);
        GIDynamicViewportVk& operator=(const GIDynamicViewportVk& rhs);
        GIDynamicViewportVk& operator=(GIDynamicViewportVk&& rhs);

        virtual ~GIDynamicViewportVk();
        virtual VkDynamicState GetType() const override final;
        virtual bool IsPendingUpdate() const override final;

        bool operator==(const GIDynamicViewportVk& rhs) const;
        void SetViewport(const VkViewport& viewport);
        const VkViewport& GetViewport() const;

    private:
        VkViewport mViewport;
        bool mIsPendingUpdate = true;
    };

    class GIDynamicScissorVk : public GIPipelineDynamicStateVk
    {
    public:
        GIDynamicScissorVk(const VkRect2D& scissor);
        GIDynamicScissorVk(const GIDynamicScissorVk& rhs);
        GIDynamicScissorVk(GIDynamicScissorVk&& rhs);
        GIDynamicScissorVk& operator=(const GIDynamicScissorVk& rhs);
        GIDynamicScissorVk& operator=(GIDynamicScissorVk&& rhs);

        virtual ~GIDynamicScissorVk();
        virtual VkDynamicState GetType() const override final;
        virtual bool IsPendingUpdate() const override final;

        bool operator==(const GIDynamicScissorVk& rhs) const;
        void SetScissor(const VkRect2D& scissor);
        const VkRect2D& GetScissor() const;

    private:
        VkRect2D mScissor;
        bool mIsPendingUpdate = false;
    };

    class GIDynamicLineWidthVk : public GIPipelineDynamicStateVk
    {
    public:
        GIDynamicLineWidthVk(float width);
        GIDynamicLineWidthVk(const GIDynamicLineWidthVk& rhs);
        GIDynamicLineWidthVk(GIDynamicLineWidthVk&& rhs);
        GIDynamicLineWidthVk& operator=(const GIDynamicLineWidthVk& rhs);
        GIDynamicLineWidthVk& operator=(GIDynamicLineWidthVk&& rhs);

        virtual ~GIDynamicLineWidthVk();
        virtual VkDynamicState GetType() const override final;
        virtual bool IsPendingUpdate() const override final;

        bool operator==(const GIDynamicLineWidthVk& rhs) const;
        void SetLineWidth(float width);
        float GetLineWidth() const;

    private:
        float mLineWidth = 1.0f;
        bool mIsPendingUpdate = false;
    };

    class GIDynamicBlendConstantsVk : public GIPipelineDynamicStateVk
    {
    public:
        GIDynamicBlendConstantsVk(const float* constants);
        GIDynamicBlendConstantsVk(const GIDynamicBlendConstantsVk& rhs);
        GIDynamicBlendConstantsVk(GIDynamicBlendConstantsVk&& rhs);
        GIDynamicBlendConstantsVk& operator=(const GIDynamicBlendConstantsVk& rhs);
        GIDynamicBlendConstantsVk& operator=(GIDynamicBlendConstantsVk&& rhs);

        virtual ~GIDynamicBlendConstantsVk();
        virtual VkDynamicState GetType() const override final;
        virtual bool IsPendingUpdate() const override final;

        bool operator==(const GIDynamicBlendConstantsVk& rhs) const;
        void SetConstants(const float* constants);
        const float* GetConstants() const;

    private:
        float mConstants[4];
        bool mIsPendingUpdate = false;
    };

    class GIDynamicDepthBiasVk : public GIPipelineDynamicStateVk
    {
    public:
        GIDynamicDepthBiasVk(float constantFactor, float clamp, float slopeFactor);
        GIDynamicDepthBiasVk(const GIDynamicDepthBiasVk& rhs);
        GIDynamicDepthBiasVk(GIDynamicDepthBiasVk&& rhs);
        GIDynamicDepthBiasVk& operator=(const GIDynamicDepthBiasVk& rhs);
        GIDynamicDepthBiasVk& operator=(GIDynamicDepthBiasVk&& rhs);

        virtual ~GIDynamicDepthBiasVk();
        virtual VkDynamicState GetType() const override final;
        virtual bool IsPendingUpdate() const override final;

        bool operator==(const GIDynamicDepthBiasVk& rhs) const;
        void SetConstantFactor(float constantFactor);
        void SetClamp(float clamp);
        void SetSlopeFactor(float slopeFactor);
        float GetConstantFactor() const;
        float GetClamp() const;
        float GetSlopeFactor() const;

    private:
        float mConstantFactor = 0.0f;
        float mClamp = 0.0f;
        float mSlopeFactor = 0.0f;
        bool mIsPendingUpdate = false;
    };

    class GIDynamicDepthBoundsVk : public GIPipelineDynamicStateVk
    {
    public:
        GIDynamicDepthBoundsVk(float minBound, float maxBound);
        GIDynamicDepthBoundsVk(const GIDynamicDepthBoundsVk& rhs);
        GIDynamicDepthBoundsVk(GIDynamicDepthBoundsVk&& rhs);
        GIDynamicDepthBoundsVk& operator=(const GIDynamicDepthBoundsVk& rhs);
        GIDynamicDepthBoundsVk& operator=(GIDynamicDepthBoundsVk&& rhs);

        virtual ~GIDynamicDepthBoundsVk();
        virtual VkDynamicState GetType() const override final;
        virtual bool IsPendingUpdate() const override final;

        bool operator==(const GIDynamicDepthBoundsVk& rhs) const;
        void SetMinBound(float minBound);
        void SetMaxBound(float maxBound);
        float GetMinBound() const;
        float GetMaxBound() const;

    private:
        float mMinBound = 0.0f;
        float mMaxBound = 1.0f;
        bool mIsPendingUpdate = false;
    };

    class GIDynamicStencilCompareMaskVk : public GIPipelineDynamicStateVk
    {
    public:
        GIDynamicStencilCompareMaskVk(VkStencilFaceFlagBits stencilFace, uint32_t compareMask);
        GIDynamicStencilCompareMaskVk(const GIDynamicStencilCompareMaskVk& rhs);
        GIDynamicStencilCompareMaskVk(GIDynamicStencilCompareMaskVk&& rhs);
        GIDynamicStencilCompareMaskVk& operator=(const GIDynamicStencilCompareMaskVk& rhs);
        GIDynamicStencilCompareMaskVk& operator=(GIDynamicStencilCompareMaskVk&& rhs);

        virtual ~GIDynamicStencilCompareMaskVk();
        virtual VkDynamicState GetType() const override final;
        virtual bool IsPendingUpdate() const override final;

        bool operator==(const GIDynamicStencilCompareMaskVk& rhs) const;
        void SetStencilFace(VkStencilFaceFlagBits stencilFace);
        void SetCompareMask(uint32_t compareMask);
        VkStencilFaceFlagBits GetStencilFace() const;
        uint32_t GetCompareMask() const;

    private:
        VkStencilFaceFlagBits mStencilFace;
        uint32_t mCompareMask = 0;
        bool mIsPendingUpdate = false;
    };

    class GIDynamicStencilWriteMaskVk : public GIPipelineDynamicStateVk
    {
    public:
        GIDynamicStencilWriteMaskVk(VkStencilFaceFlagBits stencilFace, uint32_t writeMask);
        GIDynamicStencilWriteMaskVk(const GIDynamicStencilWriteMaskVk& rhs);
        GIDynamicStencilWriteMaskVk(GIDynamicStencilWriteMaskVk&& rhs);
        GIDynamicStencilWriteMaskVk& operator=(const GIDynamicStencilWriteMaskVk& rhs);
        GIDynamicStencilWriteMaskVk& operator=(GIDynamicStencilWriteMaskVk&& rhs);

        virtual ~GIDynamicStencilWriteMaskVk();
        virtual VkDynamicState GetType() const override final;
        virtual bool IsPendingUpdate() const override final;

        bool operator==(const GIDynamicStencilWriteMaskVk& rhs) const;
        void SetStencilFace(VkStencilFaceFlagBits stencilFace);
        void SetWriteMask(uint32_t writeMask);
        VkStencilFaceFlagBits GetStencilFace() const;
        uint32_t GetWriteMask() const;

    private:
        VkStencilFaceFlagBits mStencilFace;
        uint32_t mWriteMask = 0;
        bool mIsPendingUpdate = false;
    };

    class GIDynamicStencilReferenceVk : public GIPipelineDynamicStateVk
    {
    public:
        GIDynamicStencilReferenceVk(VkStencilFaceFlagBits stencilFace, uint32_t reference);
        GIDynamicStencilReferenceVk(const GIDynamicStencilReferenceVk& rhs);
        GIDynamicStencilReferenceVk(GIDynamicStencilReferenceVk&& rhs);
        GIDynamicStencilReferenceVk& operator=(const GIDynamicStencilReferenceVk& rhs);
        GIDynamicStencilReferenceVk& operator=(GIDynamicStencilReferenceVk&& rhs);

        virtual ~GIDynamicStencilReferenceVk();
        virtual VkDynamicState GetType() const override final;
        virtual bool IsPendingUpdate() const override final;

        bool operator==(const GIDynamicStencilReferenceVk& rhs) const;
        void SetStencilFace(VkStencilFaceFlagBits stencilFace);
        void SetReference(uint32_t compareMask);
        VkStencilFaceFlagBits GetStencilFace() const;
        uint32_t GetReference() const;

    private:
        VkStencilFaceFlagBits mStencilFace;
        uint32_t mReference = 0;
        bool mIsPendingUpdate = false;
    };
}