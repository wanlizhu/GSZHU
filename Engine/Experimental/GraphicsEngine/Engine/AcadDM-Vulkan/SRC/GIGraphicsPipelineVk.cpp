#include "GIGraphicsPipelineVk.h"
#include "GIDeviceVk.h"
#include "SPIRVReflection.h"

namespace AutoCAD::Graphics::Engine
{
    GIGraphicsPipelineVk::GIGraphicsPipelineVk(
        SharedPtr<GIDeviceVk> device,
        SharedPtr<SPIRVReflection> reflection,
        const std::vector<uint32_t>& pushDescriptorSets,
        const VkGraphicsPipelineCreateInfo& createInfo,
        VkPipelineCache cache
    )
        : GIDeviceObjectVk(device)
        , mReflection(reflection)
    {
        VK_CHECK(vkCreateGraphicsPipelines(*mDevice, cache, 1, &createInfo, nullptr, &mPipeline));

        for (const auto& index : pushDescriptorSets)
            mIsPushDescriptorSets[index] = true;
        
        CreatePipelineLayout();
        CreateDescriptorPool();
    }

    GIGraphicsPipelineVk::~GIGraphicsPipelineVk()
    {
        if (IsValid())
        {
            vkDestroyPipeline(*mDevice, mPipeline, nullptr);
            mPipeline = VK_NULL_HANDLE;
        }
    }

    void GIGraphicsPipelineVk::CreatePipelineLayout()
    {
        const auto& setIndices = mReflection->GetDescriptorSetLayoutIndices();
        std::vector<VkDescriptorSetLayout> setLayouts;

        for (uint32_t index : setIndices)
        {
            mIsPushDescriptorSets[index] = IsPushDescriptorSet(index);
            const auto& setLayoutBindings = mReflection->GetDescriptorSetLayoutBindings(index);

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

        auto pushConstantRanges = mReflection->GetPushConstantRanges();

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

    void GIGraphicsPipelineVk::CreateDescriptorPool()
    {
        if (mDescriptorPools.find(std::this_thread::get_id()) == mDescriptorPools.end())
        {
            const auto& poolSizes = mReflection->GetDescriptorPoolSizes();

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

    SharedPtr<SPIRVReflection> GIGraphicsPipelineVk::GetShaderReflection() const
    {
        return mReflection;
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
            CreateDescriptorPool();
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
        , mReflection(nullptr)
    {
        mCreateInfo.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
        mCreateInfo.pNext = nullptr;
        mCreateInfo.flags = 0;

        mVertexInputState.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
        mVertexInputState.pNext = nullptr;
        mVertexInputState.flags = 0;
        mVertexInputState.vertexAttributeDescriptionCount = 0;
        mVertexInputState.pVertexAttributeDescriptions = nullptr;
        mVertexInputState.vertexBindingDescriptionCount = 0;
        mVertexInputState.pVertexBindingDescriptions = nullptr;

        mInputAssemblyState.sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
        mInputAssemblyState.pNext = nullptr;
        mInputAssemblyState.flags = 0;
        mInputAssemblyState.topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
        mInputAssemblyState.primitiveRestartEnable = VK_FALSE;

        mViewportState.sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
        mViewportState.pNext = nullptr;
        mViewportState.flags = 0;
        mViewportState.viewportCount = 0;
        mViewportState.pViewports = nullptr;
        mViewportState.scissorCount = 0;
        mViewportState.pScissors = nullptr;

        mRasterizationState.sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;
        mRasterizationState.pNext = nullptr;
        mRasterizationState.flags = 0;
        mRasterizationState.depthClampEnable = VK_FALSE;
        mRasterizationState.rasterizerDiscardEnable = VK_FALSE;
        mRasterizationState.polygonMode = VK_POLYGON_MODE_FILL;
        mRasterizationState.cullMode = VK_CULL_MODE_NONE;
        mRasterizationState.frontFace = VK_FRONT_FACE_COUNTER_CLOCKWISE;
        mRasterizationState.depthBiasEnable = VK_FALSE;
        mRasterizationState.lineWidth = 1.0f;
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
        if (mReflection == nullptr)
        {
            mReflection = SPIRVReflection::Create();
        }

        mReflection->AddShaderStage(path);
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

    GIGraphicsPipelineBuilderVk& GIGraphicsPipelineBuilderVk::AddVertexAttributeBinding(const GIVertexAttributeBindingVk& attributeBinding)
    {
        VkVertexInputBindingDescription* currentInputBinding = nullptr;
        VkVertexInputAttributeDescription* currentInputAttribute = nullptr;

        const auto& attrib = mReflection->GetVariable(attributeBinding.GetName());
        assert(attrib.has_value());
        assert(attrib.value().locationId.value() != (uint32_t)-1);
        assert(attrib.value().size <= attributeBinding.GetStride());

        // Find the same vertex input binding added before
        const auto& oldInputBinding = std::find_if(
            mVertexInputBindings.begin(),
            mVertexInputBindings.end(),
            [&](const VkVertexInputBindingDescription& desc) {
                return desc.binding == attributeBinding.GetBinding();
            });
        if (oldInputBinding != mVertexInputBindings.end())
        {
            assert(oldInputBinding->stride == attributeBinding.GetStride());
            assert(oldInputBinding->inputRate == attributeBinding.GetInputRate());
            currentInputBinding = &(*oldInputBinding);
        }
        else
        {
            mVertexInputBindings.emplace_back();
            currentInputBinding = &mVertexInputBindings.back();

            currentInputBinding->binding = attributeBinding.GetBinding();
            currentInputBinding->stride = attributeBinding.GetStride();
            currentInputBinding->inputRate = attributeBinding.GetInputRate();
        }

        // Find the same vertex attribute added before. (Normally, there shouldn't be one)
        const auto& inputAttribute = std::find_if(
            mVertexInputAttributes.begin(),
            mVertexInputAttributes.end(),
            [&](const VkVertexInputAttributeDescription& desc) {
                return desc.location == attrib.value().locationId.value();
            });
        if (inputAttribute != mVertexInputAttributes.end())
        {
            LOG_WARNING("Vertex input attribute (%d) exists, do you want to override it?\n", inputAttribute->location);
            currentInputAttribute = &(*inputAttribute);
        }
        else
        {
            mVertexInputAttributes.emplace_back();
            currentInputAttribute = &mVertexInputAttributes.back();
        }

        currentInputAttribute->binding = attributeBinding.GetBinding();
        currentInputAttribute->location = attrib.value().locationId.value();
        currentInputAttribute->offset = attributeBinding.GetOffset();
        currentInputAttribute->format = attrib.value().format;

        return *this;
    }

    GIGraphicsPipelineBuilderVk& GIGraphicsPipelineBuilderVk::AddVertexAttributeBindings(const std::vector<GIVertexAttributeBindingVk>& attributeBindings)
    {
        for (const auto& attributeBinding : attributeBindings)
        {
            AddVertexAttributeBinding(attributeBinding);
        }
        return *this;
    }

    GIGraphicsPipelineBuilderVk& GIGraphicsPipelineBuilderVk::UsePushDescriptorSetFor(uint32_t setIndex)
    {
        mPushDescriptorSets.push_back(setIndex);
        return *this;
    }

    GIGraphicsPipelineBuilderVk& GIGraphicsPipelineBuilderVk::SetInputAssemblyState(VkPrimitiveTopology topology, bool primitiveRestart)
    {
        mInputAssemblyState.topology = topology;
        mInputAssemblyState.primitiveRestartEnable = primitiveRestart ? VK_TRUE : VK_FALSE;
        return *this;
    }

    GIGraphicsPipelineBuilderVk& GIGraphicsPipelineBuilderVk::SetPatchControlPoints(uint32_t count)
    {
        if (!mTessellationState)
        {
            mTessellationState = VkPipelineTessellationStateCreateInfo();
            mTessellationState.value().sType = VK_STRUCTURE_TYPE_PIPELINE_TESSELLATION_STATE_CREATE_INFO;
            mTessellationState.value().pNext = nullptr;
            mTessellationState.value().flags = 0;
        }

        mTessellationState.value().patchControlPoints = count;
        return *this;
    }

    GIGraphicsPipelineBuilderVk& GIGraphicsPipelineBuilderVk::AddViewport(const VkViewport& viewport)
    {
        mViewports.push_back(viewport);
        
        mViewportState.viewportCount = (uint32_t)mViewports.size();
        mViewportState.pViewports = mViewports.data();
        return *this;
    }

    GIGraphicsPipelineBuilderVk& GIGraphicsPipelineBuilderVk::AddScissor(const VkRect2D& scissor)
    {
        mScissors.push_back(scissor);

        mViewportState.scissorCount = (uint32_t)mViewports.size();
        mViewportState.pScissors = mScissors.data();
        return *this;
    }

    GIGraphicsPipelineBuilderVk& GIGraphicsPipelineBuilderVk::EnableDepthClamp(bool value)
    {
        mRasterizationState.depthClampEnable = value ? VK_TRUE : VK_FALSE;
        return *this;
    }

    GIGraphicsPipelineBuilderVk& GIGraphicsPipelineBuilderVk::EnableRasterizerDiscard(bool value)
    {
        mRasterizationState.rasterizerDiscardEnable = value ? VK_TRUE : VK_FALSE;
        return *this;
    }

    GIGraphicsPipelineBuilderVk& GIGraphicsPipelineBuilderVk::SetPolygonMode(VkPolygonMode polygonMode)
    {
        mRasterizationState.polygonMode = polygonMode;
        return *this;
    }

    GIGraphicsPipelineBuilderVk& GIGraphicsPipelineBuilderVk::SetCullMode(VkCullModeFlags cullMode)
    {
        mRasterizationState.cullMode = cullMode;
        return *this;
    }

    GIGraphicsPipelineBuilderVk& GIGraphicsPipelineBuilderVk::SetFrontFace(VkFrontFace frontFace)
    {
        mRasterizationState.frontFace = frontFace;
        return *this;
    }

    GIGraphicsPipelineBuilderVk& GIGraphicsPipelineBuilderVk::EnableDepthBias(bool value)
    {
        mRasterizationState.depthBiasEnable = value ? VK_TRUE : VK_FALSE;
        return *this;
    }

    GIGraphicsPipelineBuilderVk& GIGraphicsPipelineBuilderVk::SetDepthBiasConstantFactor(float value)
    {
        mRasterizationState.depthBiasConstantFactor = value;
        return *this;
    }

    GIGraphicsPipelineBuilderVk& GIGraphicsPipelineBuilderVk::SetDepthBiasClamp(float value)
    {
        mRasterizationState.depthBiasClamp = value;
        return *this;
    }

    GIGraphicsPipelineBuilderVk& GIGraphicsPipelineBuilderVk::SetDepthBiasSlopeFactor(float value)
    {
        mRasterizationState.depthBiasSlopeFactor = value;
        return *this;
    }

    GIGraphicsPipelineBuilderVk& GIGraphicsPipelineBuilderVk::SetLineWidth(float value)
    {
        mRasterizationState.lineWidth = value;
        return *this;
    }

    GIGraphicsPipelineBuilderVk& GIGraphicsPipelineBuilderVk::AddDynamicState(VkDynamicState dynamicState)
    {
        if (mDynamicState)
        {
            mDynamicState = VkPipelineDynamicStateCreateInfo();
            mDynamicState.value().sType = VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO;
            mDynamicState.value().pNext = nullptr;
            mDynamicState.value().flags = 0;
            mDynamicState.value().dynamicStateCount = 0;
            mDynamicState.value().pDynamicStates = nullptr;
        }

        mDynamicStates.push_back(dynamicState);

        mDynamicState.value().dynamicStateCount = (uint32_t)mDynamicStates.size();
        mDynamicState.value().pDynamicStates = mDynamicStates.data();
        return *this;
    }

    SharedPtr<GIGraphicsPipelineVk> GIGraphicsPipelineBuilderVk::Build()
    {
        auto pipeline = SharedPtr<GIGraphicsPipelineVk>(new GIGraphicsPipelineVk(
            mDevice,
            mReflection,
            mPushDescriptorSets,
            mCreateInfo,
            VK_NULL_HANDLE));
        assert(pipeline->IsValid());

        return pipeline;
    }
}