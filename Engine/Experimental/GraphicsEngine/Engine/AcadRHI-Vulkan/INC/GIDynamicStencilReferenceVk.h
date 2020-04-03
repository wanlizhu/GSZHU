#pragma once

#include "GIPipelineDynamicStateVk.h"

namespace AutoCAD::Graphics::Engine
{
    class GIDynamicStencilReferenceVk : public GIPipelineDynamicStateVk
    {
    public:
        GIDynamicStencilReferenceVk(VkStencilFaceFlagBits stencilFace, uint32_t reference);
        GIDynamicStencilReferenceVk(const GIDynamicStencilReferenceVk& rhs);
        GIDynamicStencilReferenceVk(GIDynamicStencilReferenceVk&& rhs);
        GIDynamicStencilReferenceVk& operator=(const GIDynamicStencilReferenceVk& rhs);
        GIDynamicStencilReferenceVk& operator=(GIDynamicStencilReferenceVk&& rhs);

        virtual ~GIDynamicStencilReferenceVk();
        virtual VkDynamicState GetType() const override final;
        virtual bool IsPendingUpdate() const override final;

        bool operator==(const GIDynamicStencilReferenceVk& rhs) const;
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