#include "GIRenderPassVk.h"
#include "GIDeviceVk.h"

namespace AutoCAD::Graphics::Engine
{
    GIRenderPassVk::~GIRenderPassVk()
    {
        if (IsValid())
        {
            vkDestroyRenderPass(*mDevice, mRenderPassHandle, nullptr);
            mRenderPassHandle = VK_NULL_HANDLE;
        }
    }

    bool GIRenderPassVk::IsValid() const
    {
        return mRenderPassHandle != VK_NULL_HANDLE;
    }

    void GIRenderPassVk::SetDebugName(const char* name) const
    {
        SetDebugNameInternal(
            mRenderPassHandle,
            VK_DEBUG_REPORT_OBJECT_TYPE_RENDER_PASS_EXT,
            name
        );
    }

    void GIRenderPassVk::SetDebugTag(const DebugTag& tag) const
    {
        SetDebugTagInternal(
            mRenderPassHandle,
            VK_DEBUG_REPORT_OBJECT_TYPE_RENDER_PASS_EXT,
            tag
        );
    }

    GIRenderPassVk::operator const VkRenderPass& () const
    {
        return mRenderPassHandle;
    }

    uint32_t GIRenderPassVk::GetSampleCount() const
    {
        return mSampleCount;
    }

    uint32_t GIRenderPassVk::GetColorCount() const
    {
        return (uint32_t)mRenderTargetFormats.size();
    }

    VkFormat GIRenderPassVk::GetColorFormat(uint32_t index) const
    {
        assert(index < GetColorCount());
        return mRenderTargetFormats[index];
    }

    VkFormat GIRenderPassVk::GetDepthStencilFormat() const
    {
        return mDepthStencilFormat;
    }

    GIRenderPassVk::GIRenderPassVk(
        SharedPtr<GIDeviceVk> device,
        const VkRenderPassCreateInfo& createInfo
    )
        : GIDeviceObjectVk(device)
    {}

    GIRenderPassBuilderVk::GIRenderPassBuilderVk(SharedPtr<GIDeviceVk> device)
        : mDevice(device)
    {
        mCreateInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
        mCreateInfo.pNext = nullptr;
        mCreateInfo.flags = 0;
    }

    GIRenderPassBuilderVk& GIRenderPassBuilderVk::AddAttachment(const VkAttachmentDescription& attachment)
    {
        mAttachments.push_back(attachment);
        return *this;
    }

    GIRenderPassBuilderVk& GIRenderPassBuilderVk::AddAttachment(const char* name, const VkAttachmentDescription& attachment)
    {
        mAttachments.push_back(attachment);

        VkAttachmentReference reference = {};
        reference.attachment = (uint32_t)mAttachments.size() - 1;
        reference.attachment = mAttachments.back().initialLayout;
        mNamedAttachmentReferences[name] = reference;

        return *this;
    }

    /*
      If the attachment uses a color format, then loadOp and storeOp are used, and stencilLoadOp and stencilStoreOp are ignored.
      If the format has depth and/or stencil components, loadOp and storeOp apply only to the depth data,
      while stencilLoadOp and stencilStoreOp define how the stencil data is handled.
      loadOp and stencilLoadOp define the load operations that execute as part of the first subpass that uses the attachment.
      storeOp and stencilStoreOp define the store operations that execute as part of the last subpass that uses the attachment.
    */
    GIRenderPassBuilderVk& GIRenderPassBuilderVk::AddAttachment(const char* name, VkFormat format, uint32_t sampleCount)
    {
        VkAttachmentDescription attachment = {};
        attachment.flags = 0;
        attachment.format = format;
        attachment.samples = (VkSampleCountFlagBits)sampleCount;
        attachment.loadOp = VK_ATTACHMENT_LOAD_OP_LOAD;
        attachment.storeOp = VK_ATTACHMENT_STORE_OP_STORE;
        attachment.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
        attachment.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
        attachment.initialLayout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;
        attachment.finalLayout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;

        mAttachments.push_back(attachment);

        VkAttachmentReference reference = {};
        reference.attachment = (uint32_t)mAttachments.size() - 1;
        reference.attachment = mAttachments.back().initialLayout;
        mNamedAttachmentReferences[name] = reference;

        return *this;
    }

    GIRenderPassBuilderVk& GIRenderPassBuilderVk::AddSubpass(const VkSubpassDescription& subpass)
    {
        mSubpasses.push_back(subpass);
        return *this;
    }

    GIRenderPassBuilderVk& GIRenderPassBuilderVk::AddSubpass(const std::vector<const char*>& colorNames, const char* depthStencilName)
    {
        uint32_t currentId = (uint32_t)mSubpasses.size();
        auto& subpassAttachmentReferences = mSubpassAttachmentReferences[currentId];
        for (const char* colorName : colorNames)
        {
            subpassAttachmentReferences.emplace_back(mNamedAttachmentReferences.at(colorName));
        }
        if (depthStencilName != nullptr)
        {
            subpassAttachmentReferences.emplace_back(mNamedAttachmentReferences.at(depthStencilName));
        }

        VkSubpassDescription subpass = {};
        subpass.flags = 0;
        subpass.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;
        subpass.inputAttachmentCount = 0;
        subpass.pInputAttachments = nullptr;
        subpass.colorAttachmentCount = (uint32_t)colorNames.size();
        subpass.pColorAttachments = subpassAttachmentReferences.data();
        subpass.pResolveAttachments = nullptr;
        subpass.pDepthStencilAttachment = depthStencilName ? &subpassAttachmentReferences.back() : nullptr;
        subpass.preserveAttachmentCount = 0;
        subpass.pPreserveAttachments = nullptr;

        mSubpasses.emplace_back(subpass);

        return *this;
    }

    GIRenderPassBuilderVk& GIRenderPassBuilderVk::AddSubpassDependency(const VkSubpassDependency& dependency)
    {
        mDependencies.push_back(dependency);
        return *this;
    }

    SharedPtr<GIRenderPassVk> GIRenderPassBuilderVk::Build()
    {
        mCreateInfo.attachmentCount = (uint32_t)mAttachments.size();
        mCreateInfo.pAttachments = mAttachments.data();
        mCreateInfo.subpassCount = (uint32_t)mSubpasses.size();
        mCreateInfo.pSubpasses = mSubpasses.data();
        mCreateInfo.dependencyCount = (uint32_t)mDependencies.size();
        mCreateInfo.pDependencies = mDependencies.data();

        auto renderPass = SharedPtr<GIRenderPassVk>(new GIRenderPassVk(
            mDevice,
            mCreateInfo));
        assert(renderPass->IsValid());

        return renderPass;
    }
}