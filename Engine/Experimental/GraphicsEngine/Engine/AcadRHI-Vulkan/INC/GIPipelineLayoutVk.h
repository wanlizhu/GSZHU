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
            const std::vector<VkPushConstantRange>& pushConstantRanges
        );

        virtual ~GIPipelineLayoutVk();
        virtual bool IsValid() const override final;
        virtual void SetDebugName(const char* name) const override final;
        virtual void SetDebugTag(const DebugTag& tag) const override final;

        operator const VkPipelineLayout& () const;
        std::unordered_map<uint32_t, SharedPtr<GIDescriptorSetLayoutVk>> const& GetDescriptorSetLayouts() const;
        std::optional<SharedPtr<GIDescriptorSetLayoutVk>> GetDescriptorSetLayout(uint32_t setId) const;
        std::optional<VkPushConstantRange> GetPushConstantRange(VkPipelineStageFlags stage) const; // Only one push_constant block is allowed per stage.
        
    private:
        GIPipelineLayoutVk(
            SharedPtr<GIDeviceVk> device,
            const std::vector<SharedPtr<GIDescriptorSetLayoutVk>>& setLayouts,
            const std::vector<VkPushConstantRange>& pushConstantRanges
        );

    private:
        VkPipelineLayout mPipelineLayoutHandle = VK_NULL_HANDLE;
        std::unordered_map<uint32_t, SharedPtr<GIDescriptorSetLayoutVk>> mDescriptorSetLayouts; // The descriptor set ids might be uncontinuous
        std::unordered_map<VkPipelineStageFlags, VkPushConstantRange> mPushConstantRanges;
    };
}