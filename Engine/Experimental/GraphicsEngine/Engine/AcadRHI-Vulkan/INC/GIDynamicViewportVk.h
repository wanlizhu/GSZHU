#pragma once

#include "GIPipelineDynamicStateVk.h"

namespace AutoCAD::Graphics::Engine
{
    class GIDynamicViewportVk : public GIPipelineDynamicStateVk
    {
    public:
        GIDynamicViewportVk(const VkViewport& viewport);
        GIDynamicViewportVk(const GIDynamicViewportVk& rhs);
        GIDynamicViewportVk(GIDynamicViewportVk&& rhs);
        GIDynamicViewportVk& operator=(const GIDynamicViewportVk& rhs);
        GIDynamicViewportVk& operator=(GIDynamicViewportVk&& rhs);

        virtual ~GIDynamicViewportVk();
        virtual VkDynamicState GetType() const override final;
        virtual bool IsPendingUpdate() const override final;

        bool operator==(const GIDynamicViewportVk& rhs) const;
        void SetViewport(const VkViewport& viewport);
        const VkViewport& GetViewport() const;

    private:
        VkViewport mViewport;
        bool mIsPendingUpdate = true;
    };
}