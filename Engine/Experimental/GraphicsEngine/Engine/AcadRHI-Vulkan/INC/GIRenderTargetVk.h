#pragma once

#include "GIImageVk.h"

namespace AutoCAD::Graphics::Engine
{
    class GIRenderPassVk;
    class GIFramebufferVk;

    class GIRenderTargetVk : public GIImageVk
    {
    public:
        static SharedPtr<GIRenderTargetVk> Create(
            uint32_t attachmentId,
            SharedPtr<GIRenderPassVk> renderPass,
            SharedPtr<GIFramebufferVk> framebuffer
        );

        virtual ~GIRenderTargetVk();
        virtual bool IsValid() const override;

        uint32_t GetAttachmentId() const;
        VkAttachmentDescription const& GetAttachment() const;
        WeakPtr<GIRenderPassVk> GetRenderPass() const;
        WeakPtr<GIFramebufferVk> GetFramebuffer() const;

    protected:
        GIRenderTargetVk(SharedPtr<GIDeviceVk> device);

    private:
        uint32_t mAttachmentId = 0;
        VkAttachmentDescription mAttachment = {};
        WeakPtr<GIRenderPassVk> mRenderPass;
        WeakPtr<GIFramebufferVk> mFramebuffer;
    };
}