#pragma once

#include "GIDeviceObjectVk.h"

namespace AutoCAD::Graphics::Engine
{
    class GIDescriptorSetLayoutVk;

    class GIPipelineLayoutVk : public GIDeviceObjectVk
    {
        DECL_DEVICE_OBJECT(GIPipelineLayoutVk)
    public:
        static SharedPtr<GIPipelineLayoutVk> Create(
            SharedPtr<GIDeviceVk> device,
            const std::vector<SharedPtr<GIDescriptorSetLayoutVk>>& setLayouts,
            const std::vector<VkPushConstantRange>& pushConstantRanges);

        virtual ~GIPipelineLayoutVk();
        virtual bool IsValid() const override final;
        virtual void SetDebugName(const char* name) const override final;
        virtual void SetDebugTag(const DebugTag& tag) const override final;

        operator const VkPipelineLayout& () const;
        SharedPtr<GIDescriptorSetLayoutVk> GetDescriptorSetLayout(uint32_t setId) const;
        std::optional<GIRange> GetPushConstantRange(VkPipelineStageFlags stage) const; // Only one push_constant block is allowed per stage.
        
    private:
        GIPipelineLayoutVk(
            SharedPtr<GIDeviceVk> device,
            const std::vector<SharedPtr<GIDescriptorSetLayoutVk>>& setLayouts,
            const std::vector<VkPushConstantRange>& pushConstantRanges);

        GIPipelineLayoutVk(const GIPipelineLayoutVk&) = delete;
        GIPipelineLayoutVk(GIPipelineLayoutVk&&) = default;
        GIPipelineLayoutVk& operator=(const GIPipelineLayoutVk&) = delete;
        GIPipelineLayoutVk& operator=(GIPipelineLayoutVk&&) = default;

    private:
        VkPipelineLayout mPipelineLayout = VK_NULL_HANDLE;

        std::unordered_map<uint32_t, SharedPtr<GIDescriptorSetLayoutVk>> mDescriptorSetLayouts;
        std::unordered_map<VkPipelineStageFlags, GIRange> mPushConstantRanges;
    };
}