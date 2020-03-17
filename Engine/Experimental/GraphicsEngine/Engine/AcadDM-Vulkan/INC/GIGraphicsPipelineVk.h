#pragma once

#include "GIIPipelineVk.h"
#include "GIDeviceObjectVk.h"

namespace AutoCAD::Graphics::Engine
{
    class GIGraphicsPipelineVk
        : public GIDeviceObjectVk
        , public GIIPipelineVk
    {
        friend class GIGraphicsPipelineBuilderVk;
        DECL_DEVICE_OBJECT(GIGraphicsPipelineVk)
    public:
        virtual ~GIGraphicsPipelineVk();

        operator const VkPipeline& () const;
        virtual bool IsValid() const override final;
        virtual void SetDebugName(const char* name) const override final;
        virtual void SetDebugTag(const DebugTag& tag) const override final;
        virtual void SetPipelineName(const std::wstring& name) override final;
        virtual const std::wstring& GetPipelineName() const override final;

        virtual SharedPtr<SPIRVShaderProgram> GetShaderProgram() const override final;
        virtual std::vector<uint32_t> GetDescriptorSetLayoutIndices() const override final;
        virtual bool IsPushDescriptorSet(uint32_t setIndex) const override final;
        virtual std::optional<VkDescriptorSetLayout> GetDescriptorSetLayout(uint32_t setIndex) const override final;
        virtual VkDescriptorPool GetDescriptorPool() override final;
        virtual VkPipelineLayout GetPipelineLayout() const override final;
        virtual VkPipelineBindPoint GetPipelineBindPoint() const override final;

    protected:
        GIGraphicsPipelineVk(
            SharedPtr<GIDeviceVk> device,
            SharedPtr<SPIRVShaderProgram> program,
            const std::vector<uint32_t>& pushDescriptorSets,
            const VkGraphicsPipelineCreateInfo& createInfo,
            VkPipelineCache cache);

        GIGraphicsPipelineVk(const GIGraphicsPipelineVk&) = delete;
        GIGraphicsPipelineVk(GIGraphicsPipelineVk&&) = default;
        GIGraphicsPipelineVk& operator=(const GIGraphicsPipelineVk&) = delete;
        GIGraphicsPipelineVk& operator=(GIGraphicsPipelineVk&&) = default;

        void CreatePipelineLayout(const SharedPtr<SPIRVShaderProgram>& program);
        void CreateDescriptorPool(const SharedPtr<SPIRVShaderProgram>& program);

    private:
        std::wstring mPipelineName;
        VkPipeline mPipeline = VK_NULL_HANDLE;
        VkPipelineLayout mPipelineLayout = VK_NULL_HANDLE;
        SharedPtr<SPIRVShaderProgram> mShaderProgram;

        std::unordered_map<uint32_t, VkDescriptorSetLayout> mDescriptorSetLayouts;
        std::unordered_map<uint32_t, bool> mIsPushDescriptorSets;
        std::unordered_map<std::thread::id, VkDescriptorPool> mDescriptorPools; // Must be "thread_local" or "lock" involved
    };

    // TODO: create from pipeline cache
    class GIGraphicsPipelineBuilderVk
    {
    public:
        GIGraphicsPipelineBuilderVk(SharedPtr<GIDeviceVk> device);
        
        GIGraphicsPipelineBuilderVk& SetBasePipeline(VkPipeline pipeline);
        GIGraphicsPipelineBuilderVk& AddCreateFlag(VkPipelineCreateFlagBits flag);
        GIGraphicsPipelineBuilderVk& SetRenderPass(VkRenderPass renderPass, uint32_t subpass);
        GIGraphicsPipelineBuilderVk& AddShaderStage(const std::wstring& path);
        GIGraphicsPipelineBuilderVk& AddShaderStages(const std::vector<std::wstring>& paths);
        GIGraphicsPipelineBuilderVk& UsePushDescriptorSetFor(uint32_t setIndex);

        GIGraphicsPipelineBuilderVk& SetInputAssemblyState(VkPrimitiveTopology topology, bool primitiveRestart);
        GIGraphicsPipelineBuilderVk& SetPatchControlPoints(uint32_t count);
        GIGraphicsPipelineBuilderVk& AddViewport(const VkViewport& viewport);
        GIGraphicsPipelineBuilderVk& AddScissor(const VkRect2D& scissor);
        GIGraphicsPipelineBuilderVk& EnableDepthClamp(bool value);
        GIGraphicsPipelineBuilderVk& EnableRasterizerDiscard(bool value);
        GIGraphicsPipelineBuilderVk& SetPolygonMode(VkPolygonMode polygonMode);
        GIGraphicsPipelineBuilderVk& SetCullMode(VkCullModeFlags cullMode);
        GIGraphicsPipelineBuilderVk& SetFrontFace(VkFrontFace frontFace);
        GIGraphicsPipelineBuilderVk& EnableDepthBias(bool value);
        GIGraphicsPipelineBuilderVk& SetDepthBiasConstantFactor(float value);
        GIGraphicsPipelineBuilderVk& SetDepthBiasClamp(float value);
        GIGraphicsPipelineBuilderVk& SetDepthBiasSlopeFactor(float value);
        GIGraphicsPipelineBuilderVk& SetLineWidth(float value);
        GIGraphicsPipelineBuilderVk& AddDynamicState(VkDynamicState dynamicState);

        SharedPtr<GIGraphicsPipelineVk> Build();

    private:
        SharedPtr<GIDeviceVk> mDevice;
        SharedPtr<SPIRVShaderProgram> mShaderProgram;
        std::vector<uint32_t> mPushDescriptorSets;
        VkGraphicsPipelineCreateInfo mCreateInfo = {};

        // Vertex input(attribute and its vertex-buffer-binding) state
        std::vector<VkVertexInputAttributeDescription> mVertexAttributes;
        std::vector<VkVertexInputBindingDescription> mVertexBindings;
        VkPipelineVertexInputStateCreateInfo mVertexInputState = {};

        // Input assembly state
        VkPipelineInputAssemblyStateCreateInfo mInputAssemblyState;

        // Tessellation state
        std::optional<VkPipelineTessellationStateCreateInfo> mTessellationState;

        // Viewport state
        std::vector<VkViewport> mViewports;
        std::vector<VkRect2D> mScissors;
        VkPipelineViewportStateCreateInfo mViewportState;

        // Rasterization state
        VkPipelineRasterizationStateCreateInfo mRasterizationState = {};

        // Multisample state
        VkSampleMask mSampleMask = 0;
        std::optional<VkPipelineMultisampleStateCreateInfo> mMultisampleState;

        // Depth-stencil state
        std::optional<VkPipelineDepthStencilStateCreateInfo> mDepthStencilState;

        // Color blend state
        std::vector<VkPipelineColorBlendAttachmentState> mAttachmentColorBlends;
        std::optional<VkPipelineColorBlendStateCreateInfo> mColorBlendState;

        // Dynamic states
        std::vector<VkDynamicState> mDynamicStates;
        std::optional<VkPipelineDynamicStateCreateInfo> mDynamicState;
    };
}