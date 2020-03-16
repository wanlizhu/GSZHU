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
        using CacheKey = std::wstring;

        static SharedPtr<GIGraphicsPipelineVk> FindCache(CacheKey key);
        virtual ~GIGraphicsPipelineVk();

        virtual bool IsValid() const override final;
        virtual void SetDebugName(const char* name) const override final;
        virtual void SetDebugTag(const DebugTag& tag) const override final;
        virtual SharedPtr<SPIRVShaderProgram> GetShaderProgram() const override final;
        virtual VkDescriptorSetLayout GetDescriptorSetLayout() const override final;
        virtual VkDescriptorPool GetDescriptorPool() const override final;
        virtual VkPipelineLayout GetPipelineLayout() const override final;
        virtual VkPipelineBindPoint GetPipelineBindPoint() const override final;

    protected:
        GIGraphicsPipelineVk(
            SharedPtr<GIDeviceVk> device,
            SharedPtr<SPIRVShaderProgram> program,
            const VkGraphicsPipelineCreateInfo& createInfo);

        GIGraphicsPipelineVk(const GIGraphicsPipelineVk&) = delete;
        GIGraphicsPipelineVk(GIGraphicsPipelineVk&&) = default;
        GIGraphicsPipelineVk& operator=(const GIGraphicsPipelineVk&) = delete;
        GIGraphicsPipelineVk& operator=(GIGraphicsPipelineVk&&) = default;

    private:
        VkPipeline mPipeline = VK_NULL_HANDLE;
        VkPipelineLayout mPipelineLayout = VK_NULL_HANDLE;
        VkDescriptorSetLayout mDescriptorSetLayout = VK_NULL_HANDLE;
        VkDescriptorPool mDescriptorPool = VK_NULL_HANDLE;
    };

    class GIGraphicsPipelineBuilderVk
    {
    public:
        GIGraphicsPipelineBuilderVk(SharedPtr<GIDeviceVk> device);
        
        GIGraphicsPipelineBuilderVk& SetBasePipeline(SharedPtr<GIGraphicsPipelineVk> pipeline);
        GIGraphicsPipelineBuilderVk& AddCreateFlag(VkPipelineCreateFlagBits flag);
        GIGraphicsPipelineBuilderVk& SetRenderPass(VkRenderPass renderPass, uint32_t subpass);
        GIGraphicsPipelineBuilderVk& SetShaderProgram(SharedPtr<SPIRVShaderProgram> program);

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
        SharedPtr<SPIRVShaderProgram> mShaderProgram;
        VkGraphicsPipelineCreateInfo mCreateInfo = {};

        std::optional<GIVertexInputStateVk> mVertexInputState;
        std::optional<GIInputAssemblyStateVk> mInputAssemblyState;
        std::optional<GITessellationStateVk> mTessellationState;
        std::optional<GIViewportStateVk> mViewportState;
        std::optional<GIRasterizationStateVk> mRasterizationState;
        std::optional<GIMultisampleStateVk> mMultisampleState;
        std::optional<GIDepthStencilStateVk> mDepthStencilState;
        std::optional<GIColorBlendStateVk> mColorBlendState;
        std::optional<std::vector<VkDynamicState>> mDynamicStates;
    };
}