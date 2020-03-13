#pragma once

#include "GIIPipelineVk.h"
#include "GIDeviceObjectVk.h"

namespace AutoCAD::Graphics::Engine
{
    class GIIRenderStateVk;
    class GIVertexInputStateVk;
    class GIInputAssemblyStateVk;
    class GITessellationStateVk;
    class GIViewportStateVk;
    class GIRasterizationStateVk;
    class GIMultisampleStateVk;
    class GIDepthStencilStateVk;
    class GIColorBlendStateVk;

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
        GIGraphicsPipelineBuilderVk& SetStaticState(SharedPtr<GIIRenderStateVk> state);
        GIGraphicsPipelineBuilderVk& SetDynamicState(SharedPtr<GIIRenderStateVk> state);
        SharedPtr<GIGraphicsPipelineVk> Build();

    private:
        SharedPtr<GIDeviceVk> mDevice;
        VkGraphicsPipelineCreateInfo mCreateInfo = {};

        SharedPtr<GIVertexInputStateVk> mVertexInputState;
        SharedPtr<GIInputAssemblyStateVk> mInputAssemblyState;
        SharedPtr<GITessellationStateVk> mTessellationState;
        SharedPtr<GIViewportStateVk> mViewportState;
        SharedPtr<GIRasterizationStateVk> mRasterizationState;
        SharedPtr<GIMultisampleStateVk> mMultisampleState;
        SharedPtr<GIDepthStencilStateVk> mDepthStencilState;
        SharedPtr<GIColorBlendStateVk> mColorBlendState;
    };
}