#pragma once

#include "GIIPipelineVk.h"
#include "GIDeviceObjectVk.h"

namespace AutoCAD::Graphics::Engine
{
    class GIComputePipelineVk
        : public GIDeviceObjectVk
        , public GIIPipelineVk
    {
        friend class GIComputePipelineBuilderVk;
        DECL_DEVICE_OBJECT(GIComputePipelineVk)
    public:
        virtual ~GIComputePipelineVk();

        operator const VkPipeline& () const;
        virtual bool IsValid() const override final;
        virtual void SetDebugName(const char* name) const override final;
        virtual void SetDebugTag(const DebugTag& tag) const override final;
        virtual void SetPipelineName(const std::wstring& name) override final;
        virtual const std::wstring& GetPipelineName() const override final;

        virtual SharedPtr<SPIRVReflection> GetShaderReflection() const override final;
        virtual std::vector<uint32_t> GetDescriptorSetLayoutIndices() const override final;
        virtual bool IsPushDescriptorSet(uint32_t setId) const override final;
        virtual std::optional<VkDescriptorSetLayout> GetDescriptorSetLayout(uint32_t setId) const override final;
        virtual VkDescriptorPool GetDescriptorPool() override final;
        virtual VkPipelineLayout GetPipelineLayout() const override final;
        virtual VkPipelineBindPoint GetPipelineBindPoint() const override final;

    private:
        GIComputePipelineVk(
            SharedPtr<GIDeviceVk> device,
            SharedPtr<SPIRVReflection> reflection,
            const std::vector<uint32_t>& pushSetIds, // Update these descriptor sets using vkPushDescriptorSetKHR()
            const std::filesystem::path& computeShaderPath,
            const VkComputePipelineCreateInfo& createInfo,
            VkPipelineCache cache);

        GIComputePipelineVk(const GIComputePipelineVk&) = delete;
        GIComputePipelineVk(GIComputePipelineVk&&) = default;
        GIComputePipelineVk& operator=(const GIComputePipelineVk&) = delete;
        GIComputePipelineVk& operator=(GIComputePipelineVk&&) = default;

        void CreateShaderProgram(const std::filesystem::path& path);
        void CreatePipelineLayout();
        void CreateDescriptorPool();

    private:
        std::wstring mPipelineName;
        VkPipeline mPipeline = VK_NULL_HANDLE;
        VkPipelineLayout mPipelineLayout = VK_NULL_HANDLE;
        VkShaderModule mComputeShaderModule = VK_NULL_HANDLE;
        SharedPtr<SPIRVReflection> mReflection;

        std::unordered_map<uint32_t, VkDescriptorSetLayout> mDescriptorSetLayouts;
        std::unordered_map<uint32_t, bool> mIsPushDescriptorSets;
        std::unordered_map<std::thread::id, VkDescriptorPool> mDescriptorPools; // Must be "thread_local" or "lock" involved
    };

    class GIComputePipelineBuilderVk
    {
    public:
        GIComputePipelineBuilderVk(SharedPtr<GIDeviceVk> device);

        GIComputePipelineBuilderVk& SetBasePipeline(VkPipeline pipeline);
        GIComputePipelineBuilderVk& SetPipelineCache(VkPipelineCache cache);
        GIComputePipelineBuilderVk& AddCreateFlag(VkPipelineCreateFlagBits flag);
        GIComputePipelineBuilderVk& SetComputeShaderStage(const std::filesystem::path& path);
        GIComputePipelineBuilderVk& PushDescriptorSet(uint32_t setId);

        SharedPtr<GIComputePipelineVk> Build();

    private:
        SharedPtr<GIDeviceVk> mDevice;
        SharedPtr<SPIRVReflection> mReflection;
        std::filesystem::path mComputeShaderPath;
        std::vector<uint32_t> mPushDescriptorSetIds;
        VkPipelineCache mPipelineCache = VK_NULL_HANDLE;
        VkComputePipelineCreateInfo mCreateInfo = {};
    };
}