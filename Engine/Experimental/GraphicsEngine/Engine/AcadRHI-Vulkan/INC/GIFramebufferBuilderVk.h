#pragma once

#include "GICommonVk.h"

namespace AutoCAD::Graphics::Engine
{
    class GIRenderPassVk;
    class GIFramebufferVk;

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