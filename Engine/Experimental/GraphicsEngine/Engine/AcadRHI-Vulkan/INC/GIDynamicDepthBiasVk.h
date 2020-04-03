#pragma once

#include "GIPipelineDynamicStateVk.h"

namespace AutoCAD::Graphics::Engine
{
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
}