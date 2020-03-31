#pragma once

#include "GIDeviceObjectVk.h"

namespace AutoCAD::Graphics::Engine
{
    class GIShaderReflectionVk;

    class GIDescriptorSetLayoutVk : public GIDeviceObjectVk
    {
        DECL_DEVICE_OBJECT(GIDescriptorSetLayoutVk)
    public:
        static SharedPtr<GIDescriptorSetLayoutVk> Create(
            SharedPtr<GIDeviceVk> device,
            SharedPtr<GIShaderReflectionVk> reflection,
            const std::vector<VkDescriptorSetLayoutBinding>& setBindings,
            bool isPushDescriptorSet
        );

        virtual ~GIDescriptorSetLayoutVk();
        virtual bool IsValid() const override final;
        virtual void SetDebugName(const char* name) const override final;
        virtual void SetDebugTag(const DebugTag& tag) const override final;

        operator const VkDescriptorSetLayout& () const;
        bool IsPushDescriptorSet() const;
        std::unordered_map<BINDING_ID, VkDescriptorSetLayoutBinding> const& GetBindings() const;
        std::optional<VkDescriptorSetLayoutBinding> GetBindingByName(const char* name) const;

    private:
        GIDescriptorSetLayoutVk(
            SharedPtr<GIDeviceVk> device,
            SharedPtr<GIShaderReflectionVk> reflection,
            const std::vector<VkDescriptorSetLayoutBinding>& setBindings,
            bool isPushDescriptor
        );

    private:
        SharedPtr<GIShaderReflectionVk> mShaderReflection;
        VkDescriptorSetLayout mDescriptorSetLayout = VK_NULL_HANDLE;
        std::unordered_map<BINDING_ID, VkDescriptorSetLayoutBinding> mBindings; // The binding ids might be uncontinuous
        bool mIsPushDescriptorSet = false;
    };
}