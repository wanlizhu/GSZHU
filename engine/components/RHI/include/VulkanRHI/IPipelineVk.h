#pragma once

#include "RHI/IPipeline.h"
#include "VulkanRHI/VkUtilities.h"

namespace Wanlix
{
    class IPipelineVk : public IPipeline
    {
    public:
        virtual VkRenderPass GetVkRenderPass() const = 0;
        virtual VkPipeline GetVkPipeline() const = 0;
    };
}