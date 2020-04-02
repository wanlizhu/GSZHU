#pragma once

#include "GIPipelineDynamicStateVk.h"

namespace AutoCAD::Graphics::Engine
{
    class GIDynamicBlendConstantsStateVk : public GIPipelineDynamicStateVk
    {
    public:
        GIDynamicBlendConstantsStateVk(const float* constants);
        GIDynamicBlendConstantsStateVk(const GIDynamicBlendConstantsStateVk& rhs);
        GIDynamicBlendConstantsStateVk(GIDynamicBlendConstantsStateVk&& rhs);
        GIDynamicBlendConstantsStateVk& operator=(const GIDynamicBlendConstantsStateVk& rhs);
        GIDynamicBlendConstantsStateVk& operator=(GIDynamicBlendConstantsStateVk&& rhs);

        virtual ~GIDynamicBlendConstantsStateVk();
        virtual VkDynamicState GetType() const override final;
        virtual bool IsPendingUpdate() const override final;

        void SetConstants(const float* constants);
        const float* GetConstants() const;

    private:
        float mConstants[4];
        bool mIsPendingUpdate = false;
    };
}