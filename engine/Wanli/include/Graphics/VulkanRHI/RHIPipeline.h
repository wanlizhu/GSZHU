#pragma once

#include "BasicTypes.h"
#include "Graphics/VulkanRHI/RHIUtilities.h"
#include "Graphics/VulkanRHI/RHIDeviceObject.h"

namespace Wanli
{
    class RHIShader;

    struct RHIPipelineCreateInfo
    {
        VkPipelineInputAssemblyStateCreateInfo inputAssemblyState;
        VkPipelineRasterizationStateCreateInfo rasterizationState;
        VkPipelineColorBlendAttachmentState blendAttachmentStates[8];
        VkPipelineDepthStencilStateCreateInfo depthStencilState;
        VkPipelineMultisampleStateCreateInfo multisampleState;
        VkPipelineTessellationStateCreateInfo tessellationState;

        VkShaderModule vertexShaderModule = VK_NULL_HANDLE;
        VkShaderModule fragmentShaderModule = VK_NULL_HANDLE;
        VkShaderModule tessCtrlShaderModule = VK_NULL_HANDLE;
        VkShaderModule tessEvalShaderModule = VK_NULL_HANDLE;
        VkShaderModule geometryShaderModule = VK_NULL_HANDLE;

        RHIShader* shader = nullptr;
        int subpass = 0;
        int colorAttachmentCount = 1;
    };

    class DLLDECL RHIPipeline : public RHIDeviceObject
    {
    public:
        RHIPipeline(
            RHIDevice* device,
            VkPipelineCache pipelineCache,
            RHIPipelineCreateInfo& pipelineInfo,
            const Array<VkVertexInputBindingDescription>& inputBindings,
            const Array<VkVertexInputAttributeDescription>& inputAttributes,
            VkPipelineLayout pipelineLayout,
            VkRenderPass* renderPass
        );
        virtual ~RHIPipeline();

        virtual void Destroy() override;

    protected:
        VkPipeline mPipeline;
        VkPipelineLayout mPipelineLayout;
    };
}