#pragma once

#include "GIPipelineDynamicStateVk.h"

namespace AutoCAD::Graphics::Engine
{
    class GIDynamicScissorVk : public GIPipelineDynamicStateVk
    {
    public:
        GIDynamicScissorVk(const VkRect2D& scissor);
        GIDynamicScissorVk(const GIDynamicScissorVk& rhs);
        GIDynamicScissorVk(GIDynamicScissorVk&& rhs);
        GIDynamicScissorVk& operator=(const GIDynamicScissorVk& rhs);
        GIDynamicScissorVk& operator=(GIDynamicScissorVk&& rhs);

        virtual ~GIDynamicScissorVk();
        virtual VkDynamicState GetType() const override final;
        virtual bool IsPendingUpdate() const override final;

        bool operator==(const GIDynamicScissorVk& rhs) const;
        void SetScissor(const VkRect2D& scissor);
        const VkRect2D& GetScissor() const;

    private:
        VkRect2D mScissor;
        bool mIsPendingUpdate = false;
    };
}