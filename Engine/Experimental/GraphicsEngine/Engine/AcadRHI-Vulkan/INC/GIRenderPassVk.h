#pragma once

#include "GIDeviceObjectVk.h"

#define COLOR0        "Color0"
#define COLOR1        "Color1"
#define COLOR2        "Color2"
#define COLOR3        "Color3"
#define COLOR4        "Color4"
#define COLOR5        "Color5"
#define COLOR6        "Color6"
#define COLOR7        "Color7"
#define DEPTH_STENCIL "DepthStencil"

namespace AutoCAD::Graphics::Engine
{
    class GIRenderPassVk : public GIDeviceObjectVk
    {
        friend class GIRenderPassBuilderVk;
        DECL_DEVICE_OBJECT(GIRenderPassVk)
    public:
        virtual ~GIRenderPassVk();
        virtual bool IsValid() const override final;
        virtual void SetDebugName(const char* name) const override final;
        virtual void SetDebugTag(const DebugTag& tag) const override final;

        operator const VkRenderPass& () const;
        uint32_t GetSampleCount() const;
        uint32_t GetColorCount() const;
        VkFormat GetColorFormat(uint32_t index) const;
        VkFormat GetDepthStencilFormat() const;

    protected:
        GIRenderPassVk(
            SharedPtr<GIDeviceVk> device,
            const VkRenderPassCreateInfo& createInfo
        );
        
    private:
        VkRenderPass mRenderPassHandle = VK_NULL_HANDLE;
        VkSampleCountFlagBits mSampleCount = VK_SAMPLE_COUNT_1_BIT;
        VkFormat mDepthStencilFormat = VK_FORMAT_UNDEFINED;
        std::vector<VkFormat> mRenderTargetFormats;
    };

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