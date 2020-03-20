#include "GIComputePipelineVk.h"
#include "GIDeviceVk.h"
#include "SPIRVReflection.h"

namespace AutoCAD::Graphics::Engine
{
    GIComputePipelineVk::GIComputePipelineVk(
        SharedPtr<GIDeviceVk> device,
        SharedPtr<SPIRVReflection> reflection,
        const std::vector<uint32_t>& pushSetIds,
        const std::filesystem::path& computeShaderPath,
        const VkComputePipelineCreateInfo& createInfo,
        VkPipelineCache cache
    )
        : GIDeviceObjectVk(device)
        , mReflection(reflection)
    {
        for (const auto& setId : pushSetIds)
            mIsPushDescriptorSets[setId] = true;

        CreateShaderProgram(computeShaderPath);
        CreatePipelineLayout();
        CreateDescriptorPool();

        VK_CHECK(vkCreateComputePipelines(*mDevice, cache, 1, &createInfo, nullptr, &mPipeline));
    }

    GIComputePipelineVk::~GIComputePipelineVk()
    {
        if (IsValid())
        {
            for (const auto& [threadId, pool] : mDescriptorPools)
            {
                vkDestroyDescriptorPool(*mDevice, pool, nullptr);
            }

            for (const auto& [setId, setLayout] : mDescriptorSetLayouts)
            {
                vkDestroyDescriptorSetLayout(*mDevice, setLayout, nullptr);
            }

            vkDestroyShaderModule(*mDevice, mComputeShaderModule, nullptr);
            vkDestroyPipeline(*mDevice, mPipeline, nullptr);
            
            mPipeline = VK_NULL_HANDLE;
            mComputeShaderModule = VK_NULL_HANDLE;
            mDescriptorSetLayouts.clear();
            mDescriptorPools.clear();
        }
    }

    GIComputePipelineVk::operator const VkPipeline& () const
    {
        return mPipeline;
    }

    bool GIComputePipelineVk::IsValid() const
    {
        return mPipeline != VK_NULL_HANDLE;
    }

    void GIComputePipelineVk::SetDebugName(const char* name) const
    {
        SetDebugNameInternal(
            mPipeline,
            VK_DEBUG_REPORT_OBJECT_TYPE_PIPELINE_EXT,
            name);
    }

    void GIComputePipelineVk::SetDebugTag(const DebugTag& tag) const
    {
        SetDebugTagInternal(
            mPipeline,
            VK_DEBUG_REPORT_OBJECT_TYPE_PIPELINE_EXT,
            tag);
    }

    void GIComputePipelineVk::SetPipelineName(const std::wstring& name)
    {
        mPipelineName = name;
    }

    const std::wstring& GIComputePipelineVk::GetPipelineName() const
    {
        return mPipelineName;
    }

    SharedPtr<SPIRVReflection> GIComputePipelineVk::GetShaderReflection() const
    {
        return mReflection;
    }

    std::vector<uint32_t> GIComputePipelineVk::GetDescriptorSetLayoutIndices() const
    {
        std::vector<uint32_t> indices;
        for (const auto& [setId, setLayoutBindings] : mDescriptorSetLayouts)
        {
            indices.push_back(setId);
        }
        return indices;
    }

    bool GIComputePipelineVk::IsPushDescriptorSet(uint32_t setId) const
    {
        auto it = mIsPushDescriptorSets.find(setId);
        if (it == mIsPushDescriptorSets.end())
            return false;
        else
            return it->second;
    }

    std::optional<VkDescriptorSetLayout> GIComputePipelineVk::GetDescriptorSetLayout(uint32_t setId) const
    {
        auto it = mDescriptorSetLayouts.find(setId);
        if (it == mDescriptorSetLayouts.end())
            return std::nullopt;
        else
            return it->second;
    }

    VkDescriptorPool GIComputePipelineVk::GetDescriptorPool()
    {
        if (mDescriptorPools.find(std::this_thread::get_id()) == mDescriptorPools.end())
        {
            CreateDescriptorPool();
        }

        return mDescriptorPools[std::this_thread::get_id()];
    }

    VkPipelineLayout GIComputePipelineVk::GetPipelineLayout() const
    {
        return mPipelineLayout;
    }

    VkPipelineBindPoint GIComputePipelineVk::GetPipelineBindPoint() const
    {
        return VK_PIPELINE_BIND_POINT_COMPUTE;
    }

    void GIComputePipelineVk::CreateShaderProgram(const std::filesystem::path& path)
    {
        // TODO: load shader binary code and call vkCreateShaderModule()
    }

    void GIComputePipelineVk::CreatePipelineLayout()
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

    void GIComputePipelineVk::CreateDescriptorPool()
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

    GIComputePipelineBuilderVk::GIComputePipelineBuilderVk(SharedPtr<GIDeviceVk> device)
        : mDevice(device)
    {
        mCreateInfo = {};
        mCreateInfo.sType = VK_STRUCTURE_TYPE_COMPUTE_PIPELINE_CREATE_INFO;
        mCreateInfo.pNext = nullptr;
        mCreateInfo.flags = 0;
    }

    GIComputePipelineBuilderVk& GIComputePipelineBuilderVk::SetBasePipeline(VkPipeline pipeline)
    {
        mCreateInfo.basePipelineHandle = pipeline;
        mCreateInfo.flags |= VK_PIPELINE_CREATE_DERIVATIVE_BIT;
        return *this;
    }

    GIComputePipelineBuilderVk& GIComputePipelineBuilderVk::SetPipelineCache(VkPipelineCache cache)
    {
        mPipelineCache = cache;
        return *this;
    }

    GIComputePipelineBuilderVk& GIComputePipelineBuilderVk::AddCreateFlag(VkPipelineCreateFlagBits flag)
    {
        mCreateInfo.flags |= flag;
        return *this;
    }

    GIComputePipelineBuilderVk& GIComputePipelineBuilderVk::SetComputeShaderStage(const std::filesystem::path& path)
    {
        if (mReflection == nullptr)
        {
            mReflection = SPIRVReflection::Create();
        }

        mReflection->AddShaderStage(path);
        mComputeShaderPath = path;
        return *this;
    }

    GIComputePipelineBuilderVk& GIComputePipelineBuilderVk::PushDescriptorSet(uint32_t setId)
    {
        mPushDescriptorSetIds.push_back(setId);
        return *this;
    }

    SharedPtr<GIComputePipelineVk> GIComputePipelineBuilderVk::Build()
    {
        auto pipeline = SharedPtr<GIComputePipelineVk>(new GIComputePipelineVk(
            mDevice,
            mReflection,
            mPushDescriptorSetIds,
            mComputeShaderPath,
            mCreateInfo,
            mPipelineCache));
        assert(pipeline->IsValid());

        return pipeline;
    }
}