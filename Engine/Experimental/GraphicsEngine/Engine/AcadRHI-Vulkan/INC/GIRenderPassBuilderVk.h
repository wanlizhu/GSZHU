#pragma once

#include "GICommonVk.h"

namespace AutoCAD::Graphics::Engine
{
    class GIDeviceVk;
    class GIRenderPassVk;

    class GIRenderPassBuilderVk
    {
    public:
        GIRenderPassBuilderVk(SharedPtr<GIDeviceVk> device);

        GIRenderPassBuilderVk& AddAttachment(const VkAttachmentDescription& attachment);
        GIRenderPassBuilderVk& AddAttachment(const char* name, const VkAttachmentDescription& attachment);
        GIRenderPassBuilderVk& AddAttachment(const char* name, VkFormat format, uint32_t sampleCount);

        GIRenderPassBuilderVk& AddSubpass(const VkSubpassDescription& subpass);
        GIRenderPassBuilderVk& AddSubpass(const std::vector<const char*>& colorNames, const char* depthStencilName = nullptr);

        GIRenderPassBuilderVk& AddSubpassDependency(const VkSubpassDependency& dependency);
        SharedPtr<GIRenderPassVk> Build();

    private:
        SharedPtr<GIDeviceVk> mDevice;
        VkRenderPassCreateInfo mCreateInfo;

        std::vector<VkAttachmentDescription> mAttachments;
        std::vector<VkSubpassDescription> mSubpasses;
        std::vector<VkSubpassDependency> mDependencies;

        std::unordered_map<std::string, VkAttachmentReference> mNamedAttachmentReferences;
        std::unordered_map<uint32_t, std::vector<VkAttachmentReference>> mSubpassAttachmentReferences;
    };
}