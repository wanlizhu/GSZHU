#pragma once

#include "GIIPipelineVk.h"
#include "GIDeviceObjectVk.h"
#include "GIRenderStatesVk.h"

namespace AutoCAD::Graphics::Engine
{
    class GIGraphicsPipelineVk
        : public GIDeviceObjectVk
        , public GIIPipelineVk
    {
        DECL_DEVICE_OBJECT(GIGraphicsPipelineVk)
    public:
        virtual ~GIGraphicsPipelineVk();

        virtual bool IsValid() const override final;
        virtual void SetDebugName(const char* name) const override final;
        virtual void SetDebugTag(const DebugTag& tag) const override final;
        virtual SharedPtr<GIShaderProgramVk> GetShaderProgram() const override final;
        virtual VkDescriptorSetLayout GetDescriptorSetLayout() const override final;
        virtual VkDescriptorPool GetDescriptorPool() const override final;
        virtual VkPipelineLayout GetPipelineLayout() const override final;
        virtual VkPipelineBindPoint GetPipelineBindPoint() const override final;

    private:
        VkPipeline mPipeline = VK_NULL_HANDLE;
        SharedPtr<GIShaderProgramVk> mShaderProgram;
    };

    class GIGraphicsPipelineBuilderVk
    {
    public:
        GIGraphicsPipelineBuilderVk(SharedPtr<GIDeviceVk> device);
        
        GIGraphicsPipelineBuilderVk& SetBasePipeline(SharedPtr<GIGraphicsPipelineVk> pipeline);
        GIGraphicsPipelineBuilderVk& AddCreateFlag(VkPipelineCreateFlagBits flag);
        GIGraphicsPipelineBuilderVk& SetShaderProgram(SharedPtr<GIShaderProgramVk> shaderProgram);
        GIGraphicsPipelineBuilderVk& SetRenderPass(VkRenderPass renderPass, uint32_t subpass);
     
        GIGraphicsPipelineBuilderVk& SetVertexInputState(const GIIRenderStateVk& state);
        GIGraphicsPipelineBuilderVk& SetInputAssemblyState(const GIInputAssemblyStateVk& state);
        GIGraphicsPipelineBuilderVk& SetTessellationState(const GITessellationStateVk& state);
        GIGraphicsPipelineBuilderVk& SetViewportState(const GIViewportStateVk& state);
        GIGraphicsPipelineBuilderVk& SetRasterizationState(const GIRasterizationStateVk& state);
        GIGraphicsPipelineBuilderVk& SetMultisampleState(const GIMultisampleStateVk& state);
        GIGraphicsPipelineBuilderVk& SetDepthStencilState(const GIDepthStencilStateVk& state);
        GIGraphicsPipelineBuilderVk& SetColorBlendState(const GIColorBlendStateVk& state);
        GIGraphicsPipelineBuilderVk& AddDynamicState(VkDynamicState dynamicState);

        SharedPtr<GIGraphicsPipelineVk> Build();

    private:
        SharedPtr<GIDeviceVk> mDevice;
        VkGraphicsPipelineCreateInfo mCreateInfo = {};

        std::optional<GIVertexInputStateVk> mVertexInputState;
        std::optional<GIInputAssemblyStateVk> mInputAssemblyState;
        std::optional<GITessellationStateVk> mTessellationState;
        std::optional<GIViewportStateVk> mViewportState;
        std::optional<GIRasterizationStateVk> mRasterizationState;
        std::optional<GIMultisampleStateVk> mMultisampleState;
        std::optional<GIDepthStencilStateVk> mDepthStencilState;
        std::optional<GIColorBlendStateVk> mColorBlendState;
    };
}