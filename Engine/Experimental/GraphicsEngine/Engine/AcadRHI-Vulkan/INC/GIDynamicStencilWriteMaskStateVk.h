#pragma once

#include "GIPipelineDynamicStateVk.h"

namespace AutoCAD::Graphics::Engine
{
    class GIDynamicStencilWriteMaskStateVk : public GIPipelineDynamicStateVk
    {
    public:
        GIDynamicStencilWriteMaskStateVk(VkStencilFaceFlagBits stencilFace, uint32_t writeMask);
        GIDynamicStencilWriteMaskStateVk(const GIDynamicStencilWriteMaskStateVk& rhs);
        GIDynamicStencilWriteMaskStateVk(GIDynamicStencilWriteMaskStateVk&& rhs);
        GIDynamicStencilWriteMaskStateVk& operator=(const GIDynamicStencilWriteMaskStateVk& rhs);
        GIDynamicStencilWriteMaskStateVk& operator=(GIDynamicStencilWriteMaskStateVk&& rhs);

        virtual ~GIDynamicStencilWriteMaskStateVk();
        virtual VkDynamicState GetType() const override final;
        virtual bool IsPendingUpdate() const override final;

        void SetStencilFace(VkStencilFaceFlagBits stencilFace);
        void SetWriteMask(uint32_t writeMask);
        VkStencilFaceFlagBits GetStencilFace() const;
        uint32_t GetWriteMask() const;

    private:
        VkStencilFaceFlagBits mStencilFace;
        uint32_t mCompareMask = 0;
        bool mIsPendingUpdate = false;
    };
}