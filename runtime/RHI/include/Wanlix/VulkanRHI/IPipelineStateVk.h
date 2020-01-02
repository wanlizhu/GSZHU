#pragma once

#include "Wanlix/VulkanRHI/Common.h"
#include "Wanlix/RHI/IPipelineState.h"

namespace Wanlix
{
    class RHI_API IPipelineStateVk : public IPipelineState
    {
        CLASSINFO(IPipelineStateVk)
        INHERIT_SHARED_FROM_THIS(IPipelineState)
    public:
        virtual VkRenderPass GetVkRenderPass() const = 0;
        virtual VkPipeline GetVkPipeline() const = 0;
    };
}