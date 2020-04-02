#pragma once

#include "GIPipelineDynamicStateVk.h"

namespace AutoCAD::Graphics::Engine
{
    class GIDynamicStencilReferenceStateVk : public GIPipelineDynamicStateVk
    {
    public:
        GIDynamicStencilReferenceStateVk(VkStencilFaceFlagBits stencilFace, uint32_t reference);
        GIDynamicStencilReferenceStateVk(const GIDynamicStencilReferenceStateVk& rhs);
        GIDynamicStencilReferenceStateVk(GIDynamicStencilReferenceStateVk&& rhs);
        GIDynamicStencilReferenceStateVk& operator=(const GIDynamicStencilReferenceStateVk& rhs);
        GIDynamicStencilReferenceStateVk& operator=(GIDynamicStencilReferenceStateVk&& rhs);

        virtual ~GIDynamicStencilReferenceStateVk();
        virtual VkDynamicState GetType() const override final;
        virtual bool IsPendingUpdate() const override final;

        void SetStencilFace(VkStencilFaceFlagBits stencilFace);
        void SetReference(uint32_t compareMask);
        VkStencilFaceFlagBits GetStencilFace() const;
        uint32_t GetReference() const;

    private:
        VkStencilFaceFlagBits mStencilFace;
        uint32_t mReference = 0;
        bool mIsPendingUpdate = false;
    };
}