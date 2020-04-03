#pragma once

#include "GIPipelineDynamicStateVk.h"

namespace AutoCAD::Graphics::Engine
{
    class GIDynamicStencilCompareMaskVk : public GIPipelineDynamicStateVk
    {
    public:
        GIDynamicStencilCompareMaskVk(VkStencilFaceFlagBits stencilFace, uint32_t compareMask);
        GIDynamicStencilCompareMaskVk(const GIDynamicStencilCompareMaskVk& rhs);
        GIDynamicStencilCompareMaskVk(GIDynamicStencilCompareMaskVk&& rhs);
        GIDynamicStencilCompareMaskVk& operator=(const GIDynamicStencilCompareMaskVk& rhs);
        GIDynamicStencilCompareMaskVk& operator=(GIDynamicStencilCompareMaskVk&& rhs);

        virtual ~GIDynamicStencilCompareMaskVk();
        virtual VkDynamicState GetType() const override final;
        virtual bool IsPendingUpdate() const override final;

        bool operator==(const GIDynamicStencilCompareMaskVk& rhs) const;
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