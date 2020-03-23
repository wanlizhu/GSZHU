#pragma once

#include "GIRenderPassVk.h"

namespace AutoCAD::Graphics::Engine
{
    class GIFramebufferVk final : public GIDeviceObjectVk
    {
        friend class GIFramebufferBuilderVk;
        DECL_DEVICE_OBJECT(GIFramebufferVk)
    public:
        virtual ~GIFramebufferVk();
        virtual bool IsValid() const override final;
        virtual void SetDebugName(const char* name) const override final;
        virtual void SetDebugTag(const DebugTag& tag) const override final;

        operator const VkFramebuffer& () const;
        VkExtent2D GetExtent() const;
        uint32_t GetImageViewCount() const;
        std::vector<VkImageView>& GetImageViews();
        SharedPtr<GIRenderPassVk> GetRenderPass() const;
        
    protected:
        GIFramebufferVk(SharedPtr<GIRenderPassVk> renderPass, const VkFramebufferCreateInfo& createInfo);
        GIFramebufferVk(const GIFramebufferVk&) = delete;
        GIFramebufferVk(GIFramebufferVk&&) = default;
        GIFramebufferVk& operator=(const GIFramebufferVk&) = delete;
        GIFramebufferVk& operator=(GIFramebufferVk&&) = default;

    private:
        VkFramebuffer mFramebuffer = VK_NULL_HANDLE;
        VkExtent2D mExtent = { 0, 0 };
        std::vector<VkImageView> mImageViews;
        SharedPtr<GIRenderPassVk> mRenderPass;
    };

    class GIFramebufferBuilderVk
    {
    public:
        GIFramebufferBuilderVk(SharedPtr<GIRenderPassVk> renderPass);
        
        GIFramebufferBuilderVk& SetExtent(VkExtent2D extent);
        GIFramebufferBuilderVk& AddAttachment(VkImageView imageView);
        
        SharedPtr<GIFramebufferVk> Build();

    private:
        SharedPtr<GIRenderPassVk> mRenderPass;
        VkFramebufferCreateInfo mCreateInfo = {};

        std::vector<VkImageView> mImageViews;
    };
}