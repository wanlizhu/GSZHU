#include "GIFramebufferVk.h"
#include "GIDeviceVk.h"

namespace AutoCAD::Graphics::Engine
{
    GIFramebufferVk::GIFramebufferVk(SharedPtr<GIRenderPassVk> renderPass, const VkFramebufferCreateInfo& createInfo)
        : GIDeviceObjectVk(renderPass->GetDevice())
        , mRenderPass(renderPass)
    {
    
    }

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

    uint32_t GIFramebufferVk::GetImageViewCount() const
    {
        return (uint32_t)mImageViews.size();
    }

    std::vector<VkImageView>& GIFramebufferVk::GetImageViews()
    {
        return mImageViews;
    }

    SharedPtr<GIRenderPassVk> GIFramebufferVk::GetRenderPass() const
    {
        return mRenderPass;
    }
}