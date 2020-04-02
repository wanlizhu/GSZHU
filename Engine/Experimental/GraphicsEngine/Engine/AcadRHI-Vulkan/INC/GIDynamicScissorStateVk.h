#pragma once

#include "GIPipelineDynamicStateVk.h"

namespace AutoCAD::Graphics::Engine
{
    class GIDynamicScissorStateVk : public GIPipelineDynamicStateVk
    {
    public:
        GIDynamicScissorStateVk(const VkRect2D& scissor);
        GIDynamicScissorStateVk(const GIDynamicScissorStateVk& rhs);
        GIDynamicScissorStateVk(GIDynamicScissorStateVk&& rhs);
        GIDynamicScissorStateVk& operator=(const GIDynamicScissorStateVk& rhs);
        GIDynamicScissorStateVk& operator=(GIDynamicScissorStateVk&& rhs);

        virtual ~GIDynamicScissorStateVk();
        virtual VkDynamicState GetType() const override final;
        virtual bool IsPendingUpdate() const override final;

        void SetScissor(const VkRect2D& scissor);
        const VkRect2D& GetScissor() const;

    private:
        VkRect2D mScissor;
        bool mIsPendingUpdate = false;
    };
}