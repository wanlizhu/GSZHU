#pragma once

#include "GIPipelineDynamicStateVk.h"

namespace AutoCAD::Graphics::Engine
{
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
}