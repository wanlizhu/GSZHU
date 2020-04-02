#include "GIPipelineVk.h"
#include "GIDeviceVk.h"
#include "GIPipelineLayoutVk.h"
#include "GIDescriptorSetLayoutVk.h"
#include "GIShaderReflectionVk.h"
#include "GIVertexLayoutVk.h"

namespace AutoCAD::Graphics::Engine
{
    GIPipelineVk::GIPipelineVk(
        SharedPtr<GIDeviceVk> device,
        SharedPtr<GIShaderReflectionVk> reflection,
        const VkGraphicsPipelineCreateInfo& createInfo,
        VkPipelineCache cache
    )
        : GIDeviceObjectVk(device)
        , mShaderReflection(reflection)
    {
        CreatePipelineLayout();
        VK_CHECK(vkCreateGraphicsPipelines(*mDevice, cache, 1, &createInfo, nullptr, &mPipelineHandle));
    }

    GIPipelineVk::~GIPipelineVk()
    {
        if (IsValid())
        {
            vkDestroyPipeline(*mDevice, mPipelineHandle, nullptr);
            mPipelineHandle = VK_NULL_HANDLE;
        }
    }

    void GIPipelineVk::CreatePipelineLayout()
    {
        std::vector<SharedPtr<GIDescriptorSetLayoutVk>> setLayouts;
        std::vector<VkPushConstantRange> pushConstants = mShaderReflection->GetPushConstantRanges();

        for (const auto& [setId, setBindings] : mShaderReflection->GetDescriptorSetLayoutInfos())
        {
            auto setLayout = GIDescriptorSetLayoutVk::Create(
                mDevice,
                mShaderReflection,
                setBindings,
                mShaderReflection->IsPushDescriptorSet(setId)
            );
            setLayouts.push_back(setLayout);
        }

        mPipelineLayout = GIPipelineLayoutVk::Create(mDevice, setLayouts, pushConstants);
    }

    GIPipelineVk::operator const VkPipeline& () const
    {
        return mPipelineHandle;
    }

    bool GIPipelineVk::IsValid() const
    {
        return mPipelineHandle != VK_NULL_HANDLE;
    }

    void GIPipelineVk::SetDebugName(const char* name) const
    {
        SetDebugNameInternal(
            mPipelineHandle, 
            VK_DEBUG_REPORT_OBJECT_TYPE_PIPELINE_EXT,
            name
        );
    }

    void GIPipelineVk::SetDebugTag(const DebugTag& tag) const
    {
        SetDebugTagInternal(
            mPipelineHandle,
            VK_DEBUG_REPORT_OBJECT_TYPE_PIPELINE_EXT,
            tag
        );
    }

    void GIPipelineVk::SetPipelineName(const std::wstring& name)
    {
        mPipelineName = name;
    }

    const std::wstring& GIPipelineVk::GetPipelineName() const
    {
        return mPipelineName;
    }

    SharedPtr<GIShaderReflectionVk> GIPipelineVk::GetShaderReflection() const
    {
        return mShaderReflection;
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
        mPipelineCacheHandle = cache;
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
        if (mShaderReflectionBuilder == nullptr)
        {
            mShaderReflectionBuilder = SharedPtr<GIShaderReflectionBuilderVk>(new GIShaderReflectionBuilderVk());
        }
        mShaderReflectionBuilder->LoadFromFile(path);

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

    GIPipelineBuilderVk& GIPipelineBuilderVk::SetVertexLayout(SharedPtr<GIVertexLayoutVk> vertexLayout)
    {
        mVertexLayout = vertexLayout;
        mVertexInputState.vertexAttributeDescriptionCount = (uint32_t)mVertexLayout->GetVertexAttributes().size();
        mVertexInputState.pVertexAttributeDescriptions = mVertexLayout->GetVertexAttributes().data();
        mVertexInputState.vertexBindingDescriptionCount = (uint32_t)mVertexLayout->GetVertexBindings().size();
        mVertexInputState.pVertexBindingDescriptions = mVertexLayout->GetVertexBindings().data();
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

    GIPipelineBuilderVk& GIPipelineBuilderVk::SetDepthTest(bool enableTest, bool enableWrite, VkCompareOp compareOp)
    {
        if (!mDepthStencilState.has_value())
        {
            mDepthStencilState = VkPipelineDepthStencilStateCreateInfo();
            mDepthStencilState.value().sType = VK_STRUCTURE_TYPE_PIPELINE_DEPTH_STENCIL_STATE_CREATE_INFO;
            mDepthStencilState.value().pNext = nullptr;
            mDepthStencilState.value().flags = 0;
        }

        mDepthStencilState.value().depthTestEnable = enableTest;
        mDepthStencilState.value().depthWriteEnable = enableWrite;
        mDepthStencilState.value().depthCompareOp = compareOp;

        return *this;
    }

    GIPipelineBuilderVk& GIPipelineBuilderVk::SetDepthBoundsTest(bool enable, float minBound, float maxBound)
    {
        if (!mDepthStencilState.has_value())
        {
            mDepthStencilState = VkPipelineDepthStencilStateCreateInfo();
            mDepthStencilState.value().sType = VK_STRUCTURE_TYPE_PIPELINE_DEPTH_STENCIL_STATE_CREATE_INFO;
            mDepthStencilState.value().pNext = nullptr;
            mDepthStencilState.value().flags = 0;
        }

        mDepthStencilState.value().depthBoundsTestEnable = enable;
        mDepthStencilState.value().minDepthBounds = minBound;
        mDepthStencilState.value().maxDepthBounds = maxBound;

        return *this;
    }

    GIPipelineBuilderVk& GIPipelineBuilderVk::SetStencilTest(bool enableTest, const VkStencilOpState& frontFace, const VkStencilOpState& backFace)
    {
        if (!mDepthStencilState.has_value())
        {
            mDepthStencilState = VkPipelineDepthStencilStateCreateInfo();
            mDepthStencilState.value().sType = VK_STRUCTURE_TYPE_PIPELINE_DEPTH_STENCIL_STATE_CREATE_INFO;
            mDepthStencilState.value().pNext = nullptr;
            mDepthStencilState.value().flags = 0;
        }

        mDepthStencilState.value().stencilTestEnable = enableTest;
        mDepthStencilState.value().front = frontFace;
        mDepthStencilState.value().back = backFace;

        return *this;
    }

    GIPipelineBuilderVk& GIPipelineBuilderVk::SetColorBlendLogicOp(bool enable, VkLogicOp logicOp)
    {
        if (!mColorBlendState.has_value())
        {
            mColorBlendState = VkPipelineColorBlendStateCreateInfo();
            mColorBlendState.value().sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
            mColorBlendState.value().pNext = nullptr;
            mColorBlendState.value().flags = 0;
        }

        mColorBlendState.value().logicOpEnable = enable;
        mColorBlendState.value().logicOp = logicOp;

        return *this;
    }

    GIPipelineBuilderVk& GIPipelineBuilderVk::SetColorBlendConstants(const float* color)
    {
        if (!mColorBlendState.has_value())
        {
            mColorBlendState = VkPipelineColorBlendStateCreateInfo();
            mColorBlendState.value().sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
            mColorBlendState.value().pNext = nullptr;
            mColorBlendState.value().flags = 0;
        }

        std::memcpy(mColorBlendState.value().blendConstants, color, sizeof(float) * 4);

        return *this;
    }

    GIPipelineBuilderVk& GIPipelineBuilderVk::AddColorBlendAttachmentState(const VkPipelineColorBlendAttachmentState& attachmentState)
    {
        if (!mColorBlendState.has_value())
        {
            mColorBlendState = VkPipelineColorBlendStateCreateInfo();
            mColorBlendState.value().sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
            mColorBlendState.value().pNext = nullptr;
            mColorBlendState.value().flags = 0;
        }

        mAttachmentColorBlends.push_back(attachmentState);
        mColorBlendState.value().attachmentCount = (uint32_t)mAttachmentColorBlends.size();
        mColorBlendState.value().pAttachments = mAttachmentColorBlends.data();

        return *this;
    }

    GIPipelineBuilderVk& GIPipelineBuilderVk::SetSampleCount(VkSampleCountFlagBits count)
    {
        if (!mMultisampleState.has_value())
        {
            mMultisampleState = VkPipelineMultisampleStateCreateInfo();
            mMultisampleState.value().sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;
            mMultisampleState.value().pNext = nullptr;
            mMultisampleState.value().flags = 0;
        }

        mMultisampleState.value().rasterizationSamples = count;
        return *this;
    }

    GIPipelineBuilderVk& GIPipelineBuilderVk::SetSampleShading(bool enable, float minSampleShading, const VkSampleMask* sampleMask)
    {
        if (!mMultisampleState.has_value())
        {
            mMultisampleState = VkPipelineMultisampleStateCreateInfo();
            mMultisampleState.value().sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;
            mMultisampleState.value().pNext = nullptr;
            mMultisampleState.value().flags = 0;
        }

        mMultisampleState.value().sampleShadingEnable = enable;
        mMultisampleState.value().minSampleShading = minSampleShading;
        mMultisampleState.value().pSampleMask = sampleMask;

        return *this;
    }

    GIPipelineBuilderVk& GIPipelineBuilderVk::EnableAlphaToCoverage(bool value)
    {
        if (!mMultisampleState.has_value())
        {
            mMultisampleState = VkPipelineMultisampleStateCreateInfo();
            mMultisampleState.value().sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;
            mMultisampleState.value().pNext = nullptr;
            mMultisampleState.value().flags = 0;
        }

        mMultisampleState.value().alphaToCoverageEnable = value;

        return *this;
    }

    GIPipelineBuilderVk& GIPipelineBuilderVk::EnableAlphaToOneEnable(bool value)
    {
        if (!mMultisampleState.has_value())
        {
            mMultisampleState = VkPipelineMultisampleStateCreateInfo();
            mMultisampleState.value().sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;
            mMultisampleState.value().pNext = nullptr;
            mMultisampleState.value().flags = 0;
        }

        mMultisampleState.value().alphaToOneEnable = value;

        return *this;
    }

    GIPipelineBuilderVk& GIPipelineBuilderVk::SetDepthBias(bool enable, float constantFactor, float clamp, float slopeFactor)
    {
        mRasterizationState.depthBiasEnable = enable ? VK_TRUE : VK_FALSE;
        mRasterizationState.depthBiasConstantFactor = constantFactor;
        mRasterizationState.depthBiasClamp = clamp;
        mRasterizationState.depthBiasSlopeFactor = slopeFactor;
        return *this;
    }

    GIPipelineBuilderVk& GIPipelineBuilderVk::SetLineWidth(float value)
    {
        mRasterizationState.lineWidth = value;
        return *this;
    }

    GIPipelineBuilderVk& GIPipelineBuilderVk::AddDynamicState(VkDynamicState dynamicState)
    {
        if (!mDynamicState.has_value())
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
            mShaderReflectionBuilder->Build(),
            mCreateInfo,
            mPipelineCacheHandle));
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