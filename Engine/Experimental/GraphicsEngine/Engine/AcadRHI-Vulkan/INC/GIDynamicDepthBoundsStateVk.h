#pragma once

#include "GIPipelineDynamicStateVk.h"

namespace AutoCAD::Graphics::Engine
{
    class GIDynamicDepthBoundsStateVk : public GIPipelineDynamicStateVk
    {
    public:
        GIDynamicDepthBoundsStateVk(float minBound, float maxBound);
        GIDynamicDepthBoundsStateVk(const GIDynamicDepthBoundsStateVk& rhs);
        GIDynamicDepthBoundsStateVk(GIDynamicDepthBoundsStateVk&& rhs);
        GIDynamicDepthBoundsStateVk& operator=(const GIDynamicDepthBoundsStateVk& rhs);
        GIDynamicDepthBoundsStateVk& operator=(GIDynamicDepthBoundsStateVk&& rhs);

        virtual ~GIDynamicDepthBoundsStateVk();
        virtual VkDynamicState GetType() const override final;
        virtual bool IsPendingUpdate() const override final;

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