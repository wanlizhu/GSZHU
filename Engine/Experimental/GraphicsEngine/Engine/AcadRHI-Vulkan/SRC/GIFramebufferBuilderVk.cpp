#include "GIFramebufferBuilderVk.h"
#include "GIFramebufferVk.h"
#include "GIDeviceVk.h"

namespace AutoCAD::Graphics::Engine
{
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