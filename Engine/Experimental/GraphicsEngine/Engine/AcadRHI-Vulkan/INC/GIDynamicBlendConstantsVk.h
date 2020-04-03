#pragma once

#include "GIPipelineDynamicStateVk.h"

namespace AutoCAD::Graphics::Engine
{
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
}