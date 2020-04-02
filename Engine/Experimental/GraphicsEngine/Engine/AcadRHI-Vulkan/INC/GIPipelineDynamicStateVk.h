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
        bool mIsPendingUpdate = false;
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
        bool mIsPendingUpdate = false;
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
        bool mIsPendingUpdate = false;
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
        bool mIsPendingUpdate = false;
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
        bool mIsPendingUpdate = false;
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
        bool mIsPendingUpdate = false;
    };
}