#pragma once

#include "GICommonVk.h"

namespace AutoCAD::Graphics::Engine
{
    class GIVertexLayoutVk;
    class GIDeviceVk;
    class GIPipelineVk;
    class GIShaderReflectionBuilderVk;

    class GIPipelineBuilderVk
    {
    public:
        GIPipelineBuilderVk(SharedPtr<GIDeviceVk> device);
        ~GIPipelineBuilderVk();

        GIPipelineBuilderVk& SetBasePipeline(VkPipeline pipeline);
        GIPipelineBuilderVk& SetPipelineCache(VkPipelineCache cache);
        GIPipelineBuilderVk& SetBindPoint(VkPipelineBindPoint bindPoint);
        GIPipelineBuilderVk& SetVertexLayout(SharedPtr<GIVertexLayoutVk> vertexLayout);
        GIPipelineBuilderVk& SetRenderPass(VkRenderPass renderPass, uint32_t subpass);
        GIPipelineBuilderVk& AddCreateFlag(VkPipelineCreateFlagBits flag);
        GIPipelineBuilderVk& AddPushDescriptorSet(uint32_t setId);
        GIPipelineBuilderVk& AddShaderStage(const std::filesystem::path& path);

        GIPipelineBuilderVk& SetInputAssemblyState(VkPrimitiveTopology topology, bool primitiveRestart);
        GIPipelineBuilderVk& SetPatchControlPoints(uint32_t count);
        GIPipelineBuilderVk& AddViewport(const VkViewport& viewport);
        GIPipelineBuilderVk& AddScissor(const VkRect2D& scissor);
        GIPipelineBuilderVk& EnableDepthClamp(bool value);
        GIPipelineBuilderVk& EnableRasterizerDiscard(bool value);
        GIPipelineBuilderVk& SetPolygonMode(VkPolygonMode polygonMode);
        GIPipelineBuilderVk& SetCullMode(VkCullModeFlags cullMode);
        GIPipelineBuilderVk& SetFrontFace(VkFrontFace frontFace);
        GIPipelineBuilderVk& SetDepthTest(bool enableTest, bool enableWrite, VkCompareOp compareOp);
        GIPipelineBuilderVk& SetDepthBoundsTest(bool enable, float minBound, float maxBound);
        GIPipelineBuilderVk& SetStencilTest(bool enableTest, const VkStencilOpState& frontFace, const VkStencilOpState& backFace);
        GIPipelineBuilderVk& SetColorBlendLogicOp(bool enable, VkLogicOp logicOp);
        GIPipelineBuilderVk& SetColorBlendConstants(const float* color);
        GIPipelineBuilderVk& AddColorBlendAttachmentState(const VkPipelineColorBlendAttachmentState& attachmentState);
        GIPipelineBuilderVk& SetSampleCount(VkSampleCountFlagBits count);
        GIPipelineBuilderVk& SetSampleShading(bool enable, float minSampleShading, const VkSampleMask* sampleMask);
        GIPipelineBuilderVk& EnableAlphaToCoverage(bool value);
        GIPipelineBuilderVk& EnableAlphaToOneEnable(bool value);
        GIPipelineBuilderVk& SetDepthBias(bool enable, float constantFactor, float clamp, float slopeFactor);
        GIPipelineBuilderVk& SetLineWidth(float value);
        GIPipelineBuilderVk& AddDynamicState(VkDynamicState dynamicState);

        SharedPtr<GIPipelineVk> Build();

    private:
        SharedPtr<GIDeviceVk> mDevice;
        SharedPtr<GIShaderReflectionBuilderVk> mShaderReflectionBuilder;
        std::vector<uint32_t> mPushDescriptorSetIds;
        VkPipelineCache mPipelineCacheHandle = VK_NULL_HANDLE;
        VkPipelineBindPoint mBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;
        VkGraphicsPipelineCreateInfo mCreateInfo = {};

        // [1] Shader stages
        std::vector<VkShaderModule> mShaderModules; // A shader module can be destroyed while pipelines created using its shaders are still in use.
        std::vector<VkPipelineShaderStageCreateInfo> mShaderStages;

        // [2] Vertex input(attribute and its vertex-buffer-binding) state
        SharedPtr<GIVertexLayoutVk> mVertexLayout;
        VkPipelineVertexInputStateCreateInfo mVertexInputState = {};

        // [3] Input assembly state
        VkPipelineInputAssemblyStateCreateInfo mInputAssemblyState;

        // [4] Tessellation state
        std::optional<VkPipelineTessellationStateCreateInfo> mTessellationState;

        // [5] Viewport state
        std::vector<VkViewport> mViewports;
        std::vector<VkRect2D> mScissors;
        VkPipelineViewportStateCreateInfo mViewportState;

        // [6] Rasterization state
        VkPipelineRasterizationStateCreateInfo mRasterizationState = {};

        // [7] Multisample state
        std::optional<VkPipelineMultisampleStateCreateInfo> mMultisampleState;

        // [8] Depth-stencil state
        std::optional<VkPipelineDepthStencilStateCreateInfo> mDepthStencilState;

        // [9] Color blend state
        std::vector<VkPipelineColorBlendAttachmentState> mAttachmentColorBlends;
        std::optional<VkPipelineColorBlendStateCreateInfo> mColorBlendState;

        // [10] Dynamic states
        std::vector<VkDynamicState> mDynamicStates;
        std::optional<VkPipelineDynamicStateCreateInfo> mDynamicState;
    };
}