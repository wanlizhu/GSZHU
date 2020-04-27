#pragma once

#include "GIRenderPassVk.h"

namespace AutoCAD::Graphics::Engine
{
    class GIFramebufferVk final : public GIDeviceObjectVk
    {
        DECL_DEVICE_OBJECT(GIFramebufferVk)
    public:
        static SharedPtr<GIFramebufferVk> Create(
            VkExtent2D extent,
            SharedPtr<GIRenderPassVk> renderPass,
            std::vector<VkImageView> attachments
        );

        virtual ~GIFramebufferVk();
        virtual bool IsValid() const override final;
        virtual void SetDebugName(const char* name) const override final;
        virtual void SetDebugTag(const DebugTag& tag) const override final;

        operator const VkFramebuffer& () const;
        VkExtent2D GetExtent() const;
        uint32_t GetAttachmentCount() const;
        std::vector<VkImageView>& GetAttachments();
        SharedPtr<GIRenderPassVk> GetRenderPass() const;
        
    protected:
        GIFramebufferVk(SharedPtr<GIDeviceVk> device);

    private:
        VkFramebuffer mFramebufferHandle = VK_NULL_HANDLE;
        VkExtent2D mExtent = { 0, 0 };
        std::vector<VkImageView> mAttachments;
        SharedPtr<GIRenderPassVk> mRenderPass;
    };
}