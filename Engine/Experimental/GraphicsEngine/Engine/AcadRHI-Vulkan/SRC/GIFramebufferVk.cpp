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
            vkDestroyFramebuffer(*mDevice, mFramebuffer, nullptr);
            mFramebuffer = VK_NULL_HANDLE;
        }
    }

    bool GIFramebufferVk::IsValid() const
    {
        return mFramebuffer != mFramebuffer;
    }

    void GIFramebufferVk::SetDebugName(const char* name) const
    {
        SetDebugNameInternal(
            (void*)mFramebuffer,
            VK_DEBUG_REPORT_OBJECT_TYPE_FRAMEBUFFER_EXT,
            name);
    }

    void GIFramebufferVk::SetDebugTag(const DebugTag& tag) const
    {
        SetDebugTagInternal(
            (void*)mFramebuffer,
            VK_DEBUG_REPORT_OBJECT_TYPE_FRAMEBUFFER_EXT,
            tag);
    }

    GIFramebufferVk::operator const VkFramebuffer& () const
    {
        return mFramebuffer;
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

    GIFramebufferBuilderVk::GIFramebufferBuilderVk(SharedPtr<GIRenderPassVk> renderPass)
    {
        mRenderPass = renderPass;

        mCreateInfo = {};
        mCreateInfo.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
        mCreateInfo.pNext = nullptr;
        mCreateInfo.flags = 0;
        mCreateInfo.renderPass = *mRenderPass;
    }

    GIFramebufferBuilderVk& GIFramebufferBuilderVk::SetExtent(VkExtent2D extent)
    {
        mCreateInfo.width = extent.width;
        mCreateInfo.height = extent.height;
        mCreateInfo.layers = 1;

        return *this;
    }

    GIFramebufferBuilderVk& GIFramebufferBuilderVk::AddAttachment(VkImageView imageView)
    {
        mImageViews.push_back(imageView);
        return *this;
    }

    SharedPtr<GIFramebufferVk> GIFramebufferBuilderVk::Build()
    {
        mCreateInfo.attachmentCount = (uint32_t)mImageViews.size();
        mCreateInfo.pAttachments = mImageViews.data();

        auto framebuffer = SharedPtr<GIFramebufferVk>(new GIFramebufferVk(
            mRenderPass, 
            mCreateInfo));
        assert(framebuffer->IsValid());

        return framebuffer;
    }
}