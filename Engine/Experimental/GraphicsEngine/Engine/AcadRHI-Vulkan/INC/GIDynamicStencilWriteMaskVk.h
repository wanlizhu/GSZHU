#pragma once

#include "GIPipelineDynamicStateVk.h"

namespace AutoCAD::Graphics::Engine
{
    class GIDynamicStencilWriteMaskVk : public GIPipelineDynamicStateVk
    {
    public:
        GIDynamicStencilWriteMaskVk(VkStencilFaceFlagBits stencilFace, uint32_t writeMask);
        GIDynamicStencilWriteMaskVk(const GIDynamicStencilWriteMaskVk& rhs);
        GIDynamicStencilWriteMaskVk(GIDynamicStencilWriteMaskVk&& rhs);
        GIDynamicStencilWriteMaskVk& operator=(const GIDynamicStencilWriteMaskVk& rhs);
        GIDynamicStencilWriteMaskVk& operator=(GIDynamicStencilWriteMaskVk&& rhs);

        virtual ~GIDynamicStencilWriteMaskVk();
        virtual VkDynamicState GetType() const override final;
        virtual bool IsPendingUpdate() const override final;

        bool operator==(const GIDynamicStencilWriteMaskVk& rhs) const;
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