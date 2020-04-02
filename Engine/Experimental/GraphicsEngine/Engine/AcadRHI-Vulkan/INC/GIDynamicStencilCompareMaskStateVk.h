#pragma once

#include "GIPipelineDynamicStateVk.h"

namespace AutoCAD::Graphics::Engine
{
    class GIDynamicStencilCompareMaskStateVk : public GIPipelineDynamicStateVk
    {
    public:
        GIDynamicStencilCompareMaskStateVk(VkStencilFaceFlagBits stencilFace, uint32_t compareMask);
        GIDynamicStencilCompareMaskStateVk(const GIDynamicStencilCompareMaskStateVk& rhs);
        GIDynamicStencilCompareMaskStateVk(GIDynamicStencilCompareMaskStateVk&& rhs);
        GIDynamicStencilCompareMaskStateVk& operator=(const GIDynamicStencilCompareMaskStateVk& rhs);
        GIDynamicStencilCompareMaskStateVk& operator=(GIDynamicStencilCompareMaskStateVk&& rhs);

        virtual ~GIDynamicStencilCompareMaskStateVk();
        virtual VkDynamicState GetType() const override final;
        virtual bool IsPendingUpdate() const override final;

        void SetStencilFace(VkStencilFaceFlagBits stencilFace);
        void SetCompareMask(uint32_t compareMask);
        VkStencilFaceFlagBits GetStencilFace() const;
        uint32_t GetCompareMask() const;

    private:
        VkStencilFaceFlagBits mStencilFace;
        uint32_t mCompareMask = 0;
        bool mIsPendingUpdate = false;
    };
}