#pragma once

#include "GIDeviceObjectVk.h"

namespace AutoCAD::Graphics::Engine
{
    class GIDescriptorSetLayoutVk : public GIDeviceObjectVk
    {
        DECL_DEVICE_OBJECT(GIDescriptorSetLayoutVk)
    public:
        static SharedPtr<GIDescriptorSetLayoutVk> Create(
            SharedPtr<GIDeviceVk> device,
            const std::vector<VkDescriptorSetLayoutBinding>& setBindings,
            bool isPushDescriptor);

        virtual ~GIDescriptorSetLayoutVk();
        virtual bool IsValid() const override final;
        virtual void SetDebugName(const char* name) const override final;
        virtual void SetDebugTag(const DebugTag& tag) const override final;

        operator const VkDescriptorSetLayout& () const;
        bool IsPushDescriptorSet() const;
        std::optional<VkDescriptorType> GetDescriptorType(uint32_t setId) const;
        std::optional<VkShaderStageFlags> GetStageFlags(uint32_t setId) const;

    private:
        GIDescriptorSetLayoutVk(
            SharedPtr<GIDeviceVk> device,
            const std::vector<VkDescriptorSetLayoutBinding>& setBindings,
            bool isPushDescriptor);

        GIDescriptorSetLayoutVk(const GIDescriptorSetLayoutVk&) = delete;
        GIDescriptorSetLayoutVk(GIDescriptorSetLayoutVk&&) = default;
        GIDescriptorSetLayoutVk& operator=(const GIDescriptorSetLayoutVk&) = delete;
        GIDescriptorSetLayoutVk& operator=(GIDescriptorSetLayoutVk&&) = default;
        
    private:
        VkDescriptorSetLayout mDescriptorSetLayout = VK_NULL_HANDLE;
        bool mIsPushDescriptorSet = false;
        std::unordered_map<uint32_t, VkDescriptorType> mDescriptorTypes;
        std::unordered_map<uint32_t, VkShaderStageFlags> mShaderStages;
    };
}