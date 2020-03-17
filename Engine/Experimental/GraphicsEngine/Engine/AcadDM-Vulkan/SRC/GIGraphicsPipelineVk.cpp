#include "GIGraphicsPipelineVk.h"
#include "GIDeviceVk.h"
#include "SPIRVShaderProgram.h"

namespace AutoCAD::Graphics::Engine
{
    GIGraphicsPipelineVk::GIGraphicsPipelineVk(
        SharedPtr<GIDeviceVk> device,
        SharedPtr<SPIRVShaderProgram> program,
        const std::vector<uint32_t>& pushDescriptorSets,
        const VkGraphicsPipelineCreateInfo& createInfo,
        VkPipelineCache cache
    )
        : GIDeviceObjectVk(device)
    {
        VK_CHECK(vkCreateGraphicsPipelines(*mDevice, cache, 1, &createInfo, nullptr, &mPipeline));

        for (const auto& index : pushDescriptorSets)
            mIsPushDescriptorSets[index] = true;
        
        CreatePipelineLayout(program);
        CreateDescriptorPool(program);
    }

    GIGraphicsPipelineVk::~GIGraphicsPipelineVk()
    {
        if (IsValid())
        {
            vkDestroyPipeline(*mDevice, mPipeline, nullptr);
            mPipeline = VK_NULL_HANDLE;
        }
    }

    void GIGraphicsPipelineVk::CreatePipelineLayout(const SharedPtr<SPIRVShaderProgram>& program)
    {
        const auto& setIndices = program->GetDescriptorSetLayoutIndices();
        std::vector<VkDescriptorSetLayout> setLayouts;

        for (uint32_t index : setIndices)
        {
            mIsPushDescriptorSets[index] = IsPushDescriptorSet(index);
            const auto& setLayoutBindings = program->GetDescriptorSetLayoutBindings(index);

            VkDescriptorSetLayout setLayout = VK_NULL_HANDLE;
            VkDescriptorSetLayoutCreateInfo createInfo = {};
            createInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
            createInfo.pNext = nullptr;
            createInfo.flags = IsPushDescriptorSet(index) ? VK_DESCRIPTOR_SET_LAYOUT_CREATE_PUSH_DESCRIPTOR_BIT_KHR : 0;
            createInfo.bindingCount = (uint32_t)setLayoutBindings.size();
            createInfo.pBindings = setLayoutBindings.data();
            VK_CHECK(vkCreateDescriptorSetLayout(*mDevice, &createInfo, nullptr, &setLayout));

            mDescriptorSetLayouts[index] = setLayout;
            setLayouts.push_back(setLayout);
        }

        auto pushConstantRanges = program->GetPushConstantRanges();

        VkPipelineLayoutCreateInfo createInfo = {};
        createInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
        createInfo.pNext = nullptr;
        createInfo.flags = 0;
        createInfo.setLayoutCount = (uint32_t)setLayouts.size();
        createInfo.pSetLayouts = setLayouts.data();
        createInfo.pushConstantRangeCount = (uint32_t)pushConstantRanges.size();
        createInfo.pPushConstantRanges = pushConstantRanges.data();
        VK_CHECK(vkCreatePipelineLayout(*mDevice, &createInfo, nullptr, &mPipelineLayout));
    }

    void GIGraphicsPipelineVk::CreateDescriptorPool(const SharedPtr<SPIRVShaderProgram>& program)
    {
        if (mDescriptorPools.find(std::this_thread::get_id()) == mDescriptorPools.end())
        {
            const auto& poolSizes = program->GetDescriptorPoolSizes();

            VkDescriptorPool pool = VK_NULL_HANDLE;
            VkDescriptorPoolCreateInfo createInfo = {};
            createInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
            createInfo.flags = VK_DESCRIPTOR_POOL_CREATE_FREE_DESCRIPTOR_SET_BIT;
            createInfo.maxSets = 8192;
            createInfo.poolSizeCount = (uint32_t)poolSizes.size();
            createInfo.pPoolSizes = poolSizes.data();
            VK_CHECK(vkCreateDescriptorPool(*mDevice, &createInfo, nullptr, &pool));

            mDescriptorPools[std::this_thread::get_id()] = pool;
        }
    }

    GIGraphicsPipelineVk::operator const VkPipeline& () const
    {
        return mPipeline;
    }

    bool GIGraphicsPipelineVk::IsValid() const
    {
        return mPipeline != VK_NULL_HANDLE;
    }

    void GIGraphicsPipelineVk::SetDebugName(const char* name) const
    {
        SetDebugNameInternal(
            mPipeline, 
            VK_DEBUG_REPORT_OBJECT_TYPE_PIPELINE_EXT,
            name);
    }

    void GIGraphicsPipelineVk::SetDebugTag(const DebugTag& tag) const
    {
        SetDebugTagInternal(
            mPipeline,
            VK_DEBUG_REPORT_OBJECT_TYPE_PIPELINE_EXT,
            tag);
    }

    void GIGraphicsPipelineVk::SetPipelineName(const std::wstring& name)
    {
        mPipelineName = name;
    }

    const std::wstring& GIGraphicsPipelineVk::GetPipelineName() const
    {
        return mPipelineName;
    }

    SharedPtr<SPIRVShaderProgram> GIGraphicsPipelineVk::GetShaderProgram() const
    {
        return mShaderProgram;
    }

    std::vector<uint32_t> GIGraphicsPipelineVk::GetDescriptorSetLayoutIndices() const
    {
        std::vector<uint32_t> indices;
        for (const auto& [index, setLayout] : mDescriptorSetLayouts)
        {
            indices.push_back(index);
        }
        return indices;
    }

    bool GIGraphicsPipelineVk::IsPushDescriptorSet(uint32_t setIndex) const
    {
        auto it = mIsPushDescriptorSets.find(setIndex);
        if (it == mIsPushDescriptorSets.end())
            return false;
        else
            return it->second;
    }

    std::optional<VkDescriptorSetLayout> GIGraphicsPipelineVk::GetDescriptorSetLayout(uint32_t setIndex) const
    {
        auto it = mDescriptorSetLayouts.find(setIndex);
        if (it == mDescriptorSetLayouts.end())
            return std::nullopt;
        else
            return it->second;
    }

    VkDescriptorPool GIGraphicsPipelineVk::GetDescriptorPool()
    {
        if (mDescriptorPools.find(std::this_thread::get_id()) == mDescriptorPools.end())
        {
            CreateDescriptorPool(mShaderProgram);
        }

        return mDescriptorPools[std::this_thread::get_id()];
    }

    VkPipelineLayout GIGraphicsPipelineVk::GetPipelineLayout() const
    {
        return mPipelineLayout;
    }

    VkPipelineBindPoint GIGraphicsPipelineVk::GetPipelineBindPoint() const
    {
        return VK_PIPELINE_BIND_POINT_GRAPHICS;
    }

    GIGraphicsPipelineBuilderVk::GIGraphicsPipelineBuilderVk(SharedPtr<GIDeviceVk> device)
        : mDevice(device)
    {
        mCreateInfo.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
        mCreateInfo.pNext = nullptr;
        mCreateInfo.flags = 0;
    }

    /* A pipeline derivative is a child pipeline created from a parent pipeline, 
     * where the childand parent are expected to have much commonality.
     * The goal of derivative pipelines is that they be cheaper to create using the parent as a starting point, 
     * and that it be more efficient(on either host or device) to switch / bind between children of the same parent.
    */
    GIGraphicsPipelineBuilderVk& GIGraphicsPipelineBuilderVk::SetBasePipeline(VkPipeline pipeline)
    {
        // If basePipelineHandle is used, the parent pipeline must have already been created. 
        // If basePipelineIndex is used, then the parent is being created in the same command
        mCreateInfo.basePipelineHandle = pipeline;
        mCreateInfo.flags |= VK_PIPELINE_CREATE_DERIVATIVE_BIT;
        return *this;
    }

    GIGraphicsPipelineBuilderVk& GIGraphicsPipelineBuilderVk::AddCreateFlag(VkPipelineCreateFlagBits flag)
    {
        mCreateInfo.flags |= flag;
        return *this;
    }

    GIGraphicsPipelineBuilderVk& GIGraphicsPipelineBuilderVk::SetRenderPass(VkRenderPass renderPass, uint32_t subpass)
    {
        mCreateInfo.renderPass = renderPass;
        return *this;
    }

    GIGraphicsPipelineBuilderVk& GIGraphicsPipelineBuilderVk::AddShaderStage(const std::wstring& path)
    {
        if (mShaderProgram == nullptr)
        {
            mShaderProgram = SPIRVShaderProgram::Create();
        }

        auto stage = SPIRVShaderStage::Create(mDevice, path);
        assert(stage->IsValid());
        mShaderProgram->AddShaderStage(stage);

        return *this;
    }

    GIGraphicsPipelineBuilderVk& GIGraphicsPipelineBuilderVk::AddShaderStages(const std::vector<std::wstring>& paths)
    {
        for (const auto& path : paths)
        {
            AddShaderStage(path);
        }
        return *this;
    }

    GIGraphicsPipelineBuilderVk& GIGraphicsPipelineBuilderVk::UsePushDescriptorSetFor(uint32_t setIndex)
    {
        mPushDescriptorSets.push_back(setIndex);
        return *this;
    }

    GIGraphicsPipelineBuilderVk& GIGraphicsPipelineBuilderVk::SetVertexInputState(const GIIRenderStateVk& state)
    {
        
    }
    GIGraphicsPipelineBuilderVk& GIGraphicsPipelineBuilderVk::SetInputAssemblyState(const GIInputAssemblyStateVk& state);
    GIGraphicsPipelineBuilderVk& GIGraphicsPipelineBuilderVk::SetTessellationState(const GITessellationStateVk& state);
    GIGraphicsPipelineBuilderVk& GIGraphicsPipelineBuilderVk::SetViewportState(const GIViewportStateVk& state);
    GIGraphicsPipelineBuilderVk& GIGraphicsPipelineBuilderVk::SetRasterizationState(const GIRasterizationStateVk& state);
    GIGraphicsPipelineBuilderVk& GIGraphicsPipelineBuilderVk::SetMultisampleState(const GIMultisampleStateVk& state);
    GIGraphicsPipelineBuilderVk& GIGraphicsPipelineBuilderVk::SetDepthStencilState(const GIDepthStencilStateVk& state);
    GIGraphicsPipelineBuilderVk& GIGraphicsPipelineBuilderVk::SetColorBlendState(const GIColorBlendStateVk& state);
    GIGraphicsPipelineBuilderVk& GIGraphicsPipelineBuilderVk::AddDynamicState(VkDynamicState dynamicState);

    SharedPtr<GIGraphicsPipelineVk> GIGraphicsPipelineBuilderVk::Build()
    {
        auto pipeline = SharedPtr<GIGraphicsPipelineVk>(new GIGraphicsPipelineVk(
            mDevice,
            mShaderProgram,
            mPushDescriptorSets,
            mCreateInfo,
            VK_NULL_HANDLE));
        assert(pipeline->IsValid());

        return pipeline;
    }
}