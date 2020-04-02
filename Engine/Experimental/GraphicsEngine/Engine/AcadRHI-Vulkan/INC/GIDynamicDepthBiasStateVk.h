#pragma once

#include "GIPipelineDynamicStateVk.h"

namespace AutoCAD::Graphics::Engine
{
    class GIDynamicDepthBiasStateVk : public GIPipelineDynamicStateVk
    {
    public:
        GIDynamicDepthBiasStateVk(float constantFactor, float clamp, float slopeFactor);
        GIDynamicDepthBiasStateVk(const GIDynamicDepthBiasStateVk& rhs);
        GIDynamicDepthBiasStateVk(GIDynamicDepthBiasStateVk&& rhs);
        GIDynamicDepthBiasStateVk& operator=(const GIDynamicDepthBiasStateVk& rhs);
        GIDynamicDepthBiasStateVk& operator=(GIDynamicDepthBiasStateVk&& rhs);

        virtual ~GIDynamicDepthBiasStateVk();
        virtual VkDynamicState GetType() const override final;
        virtual bool IsPendingUpdate() const override final;

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
}