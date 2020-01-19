#pragma once

#include "RHI/IPipelineState.h"
#include "VulkanRHI/VkUtilities.h"

namespace Wanlix
{
    class IPipelineStateVk : public IPipelineState
    {
    public:
        virtual VkRenderPass GetVkRenderPass() const = 0;
        virtual VkPipeline GetVkPipeline() const = 0;
    };
}