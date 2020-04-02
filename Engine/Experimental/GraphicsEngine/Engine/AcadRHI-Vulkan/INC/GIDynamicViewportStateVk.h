#pragma once

#include "GIPipelineDynamicStateVk.h"

namespace AutoCAD::Graphics::Engine
{
    class GIDynamicViewportStateVk : public GIPipelineDynamicStateVk
    {
    public:
        GIDynamicViewportStateVk(const VkViewport& viewport);
        GIDynamicViewportStateVk(const GIDynamicViewportStateVk& rhs);
        GIDynamicViewportStateVk(GIDynamicViewportStateVk&& rhs);
        GIDynamicViewportStateVk& operator=(const GIDynamicViewportStateVk& rhs);
        GIDynamicViewportStateVk& operator=(GIDynamicViewportStateVk&& rhs);

        virtual ~GIDynamicViewportStateVk() = default;
        virtual VkDynamicState GetType() const override final;
        virtual bool IsPendingUpdate() const override final;

        void SetViewport(const VkViewport& viewport);
        const VkViewport& GetViewport() const;

    private:
        VkViewport mViewport;
        bool mIsPendingUpdate = false;
    };
}