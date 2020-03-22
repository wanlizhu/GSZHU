#include "GIPipelineVk.h"
#include "GIDeviceVk.h"
#include "GIPipelineLayoutVk.h"
#include "GIDescriptorSetLayoutVk.h"
#include "SPIRVReflection.h"

namespace AutoCAD::Graphics::Engine
{
    GIPipelineVk::GIPipelineVk(
        SharedPtr<GIDeviceVk> device,
        SharedPtr<SPIRVReflection> reflection,
        const VkGraphicsPipelineCreateInfo& createInfo,
        VkPipelineCache cache
    )
        : GIDeviceObjectVk(device)
        , mReflection(reflection)
    {
        CreatePipelineLayout();
        VK_CHECK(vkCreateGraphicsPipelines(*mDevice, cache, 1, &createInfo, nullptr, &mPipeline));
    }

    GIPipelineVk::~GIPipelineVk()
    {
        if (IsValid())
        {
            vkDestroyPipeline(*mDevice, mPipeline, nullptr);
            mPipeline = VK_NULL_HANDLE;
        }
    }

    void GIPipelineVk::CreatePipelineLayout()
    {
        std::vector<SharedPtr<GIDescriptorSetLayoutVk>> setLayouts;
        std::vector<VkPushConstantRange> pushConstants = mReflection->GetPushConstantRanges();

        for (const auto& [setId, setBindings] : mReflection->GetDescriptorSetLayoutsBindings())
        {
            auto setLayout = GIDescriptorSetLayoutVk::Create(mDevice, setBindings, mReflection->IsPushDescriptorSet(setId));
            setLayouts.push_back(setLayout);
        }

        mPipelineLayout = GIPipelineLayoutVk::Create(mDevice, setLayouts, pushConstants);
    }

    GIPipelineVk::operator const VkPipeline& () const
    {
        return mPipeline;
    }

    bool GIPipelineVk::IsValid() const
    {
        return mPipeline != VK_NULL_HANDLE;
    }

    void GIPipelineVk::SetDebugName(const char* name) const
    {
        SetDebugNameInternal(
            mPipeline, 
            VK_DEBUG_REPORT_OBJECT_TYPE_PIPELINE_EXT,
            name);
    }

    void GIPipelineVk::SetDebugTag(const DebugTag& tag) const
    {
        SetDebugTagInternal(
            mPipeline,
            VK_DEBUG_REPORT_OBJECT_TYPE_PIPELINE_EXT,
            tag);
    }

    void GIPipelineVk::SetPipelineName(const std::wstring& name)
    {
        mPipelineName = name;
    }

    const std::wstring& GIPipelineVk::GetPipelineName() const
    {
        return mPipelineName;
    }

    SharedPtr<SPIRVReflection> GIPipelineVk::GetShaderReflection() const
    {
        return mReflection;
    }

    SharedPtr<GIPipelineLayoutVk> GIPipelineVk::GetPipelineLayout() const
    {
        return mPipelineLayout;
    }

    VkPipelineBindPoint GIPipelineVk::GetPipelineBindPoint() const
    {
        return mPipelineBindPoint;
    }

    GIPipelineBuilderVk::GIPipelineBuilderVk(SharedPtr<GIDeviceVk> device)
        : mDevice(device)
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

    GIPipelineBuilderVk::~GIPipelineBuilderVk()
    {
        for (const auto& shaderModule : mShaderModules)
        {
            vkDestroyShaderModule(*mDevice, shaderModule, nullptr);
        }

        mShaderModules.clear();
    }

    /* A pipeline derivative is a child pipeline created from a parent pipeline, 
     * where the childand parent are expected to have much commonality.
     * The goal of derivative pipelines is that they be cheaper to create using the parent as a starting point, 
     * and that it be more efficient(on either host or device) to switch / bind between children of the same parent.
    */
    GIPipelineBuilderVk& GIPipelineBuilderVk::SetBasePipeline(VkPipeline pipeline)
    {
        // If basePipelineHandle is used, the parent pipeline must have already been created. 
        // If basePipelineIndex is used, then the parent is being created in the same command
        mCreateInfo.basePipelineHandle = pipeline;
        mCreateInfo.flags |= VK_PIPELINE_CREATE_DERIVATIVE_BIT;
        return *this;
    }

    GIPipelineBuilderVk& GIPipelineBuilderVk::SetPipelineCache(VkPipelineCache cache)
    {
        mPipelineCache = cache;
        return *this;
    }

    GIPipelineBuilderVk& GIPipelineBuilderVk::SetBindPoint(VkPipelineBindPoint bindPoint)
    {
        mBindPoint = bindPoint;
        return *this;
    }

    GIPipelineBuilderVk& GIPipelineBuilderVk::AddCreateFlag(VkPipelineCreateFlagBits flag)
    {
        mCreateInfo.flags |= flag;
        return *this;
    }

    GIPipelineBuilderVk& GIPipelineBuilderVk::SetRenderPass(VkRenderPass renderPass, uint32_t subpass)
    {
        mCreateInfo.renderPass = renderPass;
        return *this;
    }

    GIPipelineBuilderVk& GIPipelineBuilderVk::AddShaderStage(const std::filesystem::path& path)
    {
        if (mReflection == nullptr)
        {
            mReflection = SPIRVReflection::Create();
        }
        mReflection->AddShaderStage(path);

        std::vector<char> buffer;
        std::ifstream file(path.wstring(), std::ios::binary);
        file.unsetf(std::ios::skipws); // According to my test, std::ios::binary is not being honored
        file.seekg(0, std::ios::end);
        buffer.resize(file.tellg());
        file.seekg(0, std::ios::beg);
        file.read(buffer.data(), buffer.size());
        
        VkShaderModule shaderModule = VK_NULL_HANDLE;
        VkShaderModuleCreateInfo createInfo = {};
        createInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
        createInfo.pNext = nullptr;
        createInfo.flags = 0;
        createInfo.codeSize = (uint32_t)buffer.size();
        createInfo.pCode = (const uint32_t*)buffer.data();

        VK_CHECK(vkCreateShaderModule(*mDevice, &createInfo, nullptr, &shaderModule));
        mShaderModules.push_back(shaderModule);

        mCreateInfo.stageCount = (uint32_t)mShaderStages.size();
        mCreateInfo.pStages = mShaderStages.data();

        return *this;
    }

    GIPipelineBuilderVk& GIPipelineBuilderVk::AddShaderStages(const std::vector<std::filesystem::path>& paths)
    {
        for (const auto& path : paths)
        {
            AddShaderStage(path);
        }
        return *this;
    }

    GIPipelineBuilderVk& GIPipelineBuilderVk::AddVertexAttributeBinding(const GIVertexAttributeBindingVk& attributeBinding)
    {
        VkVertexInputBindingDescription* currentInputBinding = nullptr;
        VkVertexInputAttributeDescription* currentInputAttribute = nullptr;

        const auto& attribs = mReflection->GetAttributes();
        assert(attribs.size() >= 1);

        auto it = attribs.find(attributeBinding.GetName());
        if (it == attribs.end())
        {
            LOG_ERROR("Failed to find vertex attribute \"%s\"\n", attributeBinding.GetName().c_str());
            return *this;
        }

        const SPIRVAttribute& attrib = it->second;
        assert(attrib.locationId != (uint32_t)-1);
        assert(attrib.size <= attributeBinding.GetStride());

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
                return desc.location == attrib.locationId;
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
        currentInputAttribute->location = attrib.locationId;
        currentInputAttribute->offset = attributeBinding.GetOffset();
        currentInputAttribute->format = attrib.format;

        return *this;
    }

    GIPipelineBuilderVk& GIPipelineBuilderVk::AddVertexAttributeBindings(const std::vector<GIVertexAttributeBindingVk>& attributeBindings)
    {
        for (const auto& attributeBinding : attributeBindings)
        {
            AddVertexAttributeBinding(attributeBinding);
        }
        return *this;
    }

    GIPipelineBuilderVk& GIPipelineBuilderVk::AddPushDescriptorSet(uint32_t setId)
    {
        mPushDescriptorSetIds.push_back(setId);
        return *this;
    }

    GIPipelineBuilderVk& GIPipelineBuilderVk::SetInputAssemblyState(VkPrimitiveTopology topology, bool primitiveRestart)
    {
        mInputAssemblyState.topology = topology;
        mInputAssemblyState.primitiveRestartEnable = primitiveRestart ? VK_TRUE : VK_FALSE;
        return *this;
    }

    GIPipelineBuilderVk& GIPipelineBuilderVk::SetPatchControlPoints(uint32_t count)
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

    GIPipelineBuilderVk& GIPipelineBuilderVk::AddViewport(const VkViewport& viewport)
    {
        mViewports.push_back(viewport);
        
        mViewportState.viewportCount = (uint32_t)mViewports.size();
        mViewportState.pViewports = mViewports.data();
        return *this;
    }

    GIPipelineBuilderVk& GIPipelineBuilderVk::AddScissor(const VkRect2D& scissor)
    {
        mScissors.push_back(scissor);

        mViewportState.scissorCount = (uint32_t)mViewports.size();
        mViewportState.pScissors = mScissors.data();
        return *this;
    }

    GIPipelineBuilderVk& GIPipelineBuilderVk::EnableDepthClamp(bool value)
    {
        mRasterizationState.depthClampEnable = value ? VK_TRUE : VK_FALSE;
        return *this;
    }

    GIPipelineBuilderVk& GIPipelineBuilderVk::EnableRasterizerDiscard(bool value)
    {
        mRasterizationState.rasterizerDiscardEnable = value ? VK_TRUE : VK_FALSE;
        return *this;
    }

    GIPipelineBuilderVk& GIPipelineBuilderVk::SetPolygonMode(VkPolygonMode polygonMode)
    {
        mRasterizationState.polygonMode = polygonMode;
        return *this;
    }

    GIPipelineBuilderVk& GIPipelineBuilderVk::SetCullMode(VkCullModeFlags cullMode)
    {
        mRasterizationState.cullMode = cullMode;
        return *this;
    }

    GIPipelineBuilderVk& GIPipelineBuilderVk::SetFrontFace(VkFrontFace frontFace)
    {
        mRasterizationState.frontFace = frontFace;
        return *this;
    }

    GIPipelineBuilderVk& GIPipelineBuilderVk::EnableDepthBias(bool value)
    {
        mRasterizationState.depthBiasEnable = value ? VK_TRUE : VK_FALSE;
        return *this;
    }

    GIPipelineBuilderVk& GIPipelineBuilderVk::SetDepthBiasConstantFactor(float value)
    {
        mRasterizationState.depthBiasConstantFactor = value;
        return *this;
    }

    GIPipelineBuilderVk& GIPipelineBuilderVk::SetDepthBiasClamp(float value)
    {
        mRasterizationState.depthBiasClamp = value;
        return *this;
    }

    GIPipelineBuilderVk& GIPipelineBuilderVk::SetDepthBiasSlopeFactor(float value)
    {
        mRasterizationState.depthBiasSlopeFactor = value;
        return *this;
    }

    GIPipelineBuilderVk& GIPipelineBuilderVk::SetLineWidth(float value)
    {
        mRasterizationState.lineWidth = value;
        return *this;
    }

    GIPipelineBuilderVk& GIPipelineBuilderVk::AddDynamicState(VkDynamicState dynamicState)
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

    SharedPtr<GIPipelineVk> GIPipelineBuilderVk::Build()
    {
        auto pipeline = SharedPtr<GIPipelineVk>(new GIPipelineVk(
            mDevice,
            mReflection,
            mCreateInfo,
            mPipelineCache));
        assert(pipeline->IsValid());

        return pipeline;
    }

    GIVertexAttributeBindingVk::GIVertexAttributeBindingVk(
        const std::string& name,
        uint32_t binding,
        uint32_t offset,
        uint32_t stride,
        VkVertexInputRate inputRate
    )
        : mAttributeName(name)
        , mVertexBufferBindingID(binding)
        , mOffset(offset)
        , mStride(stride)
        , mInputRate(inputRate)
    {}

    std::string const& GIVertexAttributeBindingVk::GetName() const
    {
        return mAttributeName;
    }

    uint32_t GIVertexAttributeBindingVk::GetBinding() const
    {
        return mVertexBufferBindingID;
    }

    uint32_t GIVertexAttributeBindingVk::GetOffset() const
    {
        return mOffset;
    }

    uint32_t GIVertexAttributeBindingVk::GetStride() const
    {
        return mStride;
    }

    VkVertexInputRate GIVertexAttributeBindingVk::GetInputRate() const
    {
        return mInputRate;
    }

}