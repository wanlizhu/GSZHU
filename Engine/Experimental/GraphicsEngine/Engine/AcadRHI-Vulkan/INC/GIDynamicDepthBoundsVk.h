#pragma once

#include "GIPipelineDynamicStateVk.h"

namespace AutoCAD::Graphics::Engine
{
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
}