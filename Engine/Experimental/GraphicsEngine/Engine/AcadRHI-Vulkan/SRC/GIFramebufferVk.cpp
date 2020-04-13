#include "GIFramebufferVk.h"
#include "GIDeviceVk.h"

namespace AutoCAD::Graphics::Engine
{
    SharedPtr<GIFramebufferVk> GIFramebufferVk::Create(
        VkExtent2D extent,
        SharedPtr<GIRenderPassVk> renderPass,
        std::vector<VkImageView> attachments
    )
    {
        auto device = renderPass->GetDevice();
        VkFramebuffer framebuffer = VK_NULL_HANDLE;

        VkFramebufferCreateInfo createInfo = {};
        createInfo.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
        createInfo.pNext = nullptr;
        createInfo.flags = 0;
        createInfo.renderPass = *renderPass;
        createInfo.attachmentCount = (uint32_t)attachments.size();
        createInfo.pAttachments = attachments.data();
        createInfo.width = extent.width;
        createInfo.height = extent.height;
        createInfo.layers = 1;

        VK_CHECK(vkCreateFramebuffer(*device, &createInfo, nullptr, &framebuffer));

        SharedPtr<GIFramebufferVk> result(new GIFramebufferVk(device));
        result->mFramebufferHandle = framebuffer;
        result->mExtent = extent;
        result->mRenderPass = renderPass;
        result->mAttachments = attachments;

        return result;
    }

    GIFramebufferVk::GIFramebufferVk(SharedPtr<GIDeviceVk> device)
        : GIDeviceObjectVk(device)
    {}

    GIFramebufferVk::~GIFramebufferVk()
    {
        if (IsValid())
        {
            vkDestroyFramebuffer(*mDevice, mFramebufferHandle, nullptr);
            mFramebufferHandle = VK_NULL_HANDLE;
        }
    }

    bool GIFramebufferVk::IsValid() const
    {
        return mFramebufferHandle != VK_NULL_HANDLE;
    }

    void GIFramebufferVk::SetDebugName(const char* name) const
    {
        SetDebugNameInternal(
            (void*)mFramebufferHandle,
            VK_DEBUG_REPORT_OBJECT_TYPE_FRAMEBUFFER_EXT,
            name
        );
    }

    void GIFramebufferVk::SetDebugTag(const DebugTag& tag) const
    {
        SetDebugTagInternal(
            (void*)mFramebufferHandle,
            VK_DEBUG_REPORT_OBJECT_TYPE_FRAMEBUFFER_EXT,
            tag
        );
    }

    GIFramebufferVk::operator const VkFramebuffer& () const
    {
        return mFramebufferHandle;
    }

    VkExtent2D GIFramebufferVk::GetExtent() const
    {
        return mExtent;
    }

    uint32_t GIFramebufferVk::GetAttachmentCount() const
    {
        return (uint32_t)mAttachments.size();
    }

    std::vector<VkImageView>& GIFramebufferVk::GetAttachments()
    {
        return mAttachments;
    }

    SharedPtr<GIRenderPassVk> GIFramebufferVk::GetRenderPass() const
    {
        return mRenderPass;
    }
}