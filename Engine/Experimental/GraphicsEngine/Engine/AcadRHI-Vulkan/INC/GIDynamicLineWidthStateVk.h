#pragma once

#include "GIPipelineDynamicStateVk.h"

namespace AutoCAD::Graphics::Engine
{
    class GIDynamicLineWidthStateVk : public GIPipelineDynamicStateVk
    {
    public:
        GIDynamicLineWidthStateVk(float width);
        GIDynamicLineWidthStateVk(const GIDynamicLineWidthStateVk& rhs);
        GIDynamicLineWidthStateVk(GIDynamicLineWidthStateVk&& rhs);
        GIDynamicLineWidthStateVk& operator=(const GIDynamicLineWidthStateVk& rhs);
        GIDynamicLineWidthStateVk& operator=(GIDynamicLineWidthStateVk&& rhs);

        virtual ~GIDynamicLineWidthStateVk();
        virtual VkDynamicState GetType() const override final;
        virtual bool IsPendingUpdate() const override final;

        void SetLineWidth(float width);
        float GetLineWidth() const;

    private:
        float mLineWidth = 1.0f;
        bool mIsPendingUpdate = false;
    };
}