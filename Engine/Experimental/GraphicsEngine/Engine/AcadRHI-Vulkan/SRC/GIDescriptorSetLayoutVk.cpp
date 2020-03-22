#include "GIDescriptorSetLayoutVk.h"
#include "GIDeviceVk.h"

namespace AutoCAD::Graphics::Engine
{
    SharedPtr<GIDescriptorSetLayoutVk> GIDescriptorSetLayoutVk::Create(
        SharedPtr<GIDeviceVk> device,
        const std::vector<VkDescriptorSetLayoutBinding>& setBindings,
        bool isPushDescriptor)
    {
        return SharedPtr<GIDescriptorSetLayoutVk>(new GIDescriptorSetLayoutVk(device, setBindings, isPushDescriptor));
    }

    GIDescriptorSetLayoutVk::GIDescriptorSetLayoutVk(
        SharedPtr<GIDeviceVk> device,
        const std::vector<VkDescriptorSetLayoutBinding>& setBindings,
        bool isPushDescriptor
        )
        : GIDeviceObjectVk(device)
        , mIsPushDescriptorSet(isPushDescriptor)
    {
        VkDescriptorSetLayoutCreateInfo createInfo = {};
        createInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
        createInfo.pNext = nullptr;
        createInfo.flags = isPushDescriptor ? VK_DESCRIPTOR_SET_LAYOUT_CREATE_PUSH_DESCRIPTOR_BIT_KHR : 0;
        createInfo.bindingCount = (uint32_t)setBindings.size();
        createInfo.pBindings = setBindings.data();
        VK_CHECK(vkCreateDescriptorSetLayout(*device, &createInfo, nullptr, &mDescriptorSetLayout));

        for (const auto& binding : setBindings)
        {
            mDescriptorTypes[binding.binding] = binding.descriptorType;
            mShaderStages[binding.binding] = binding.stageFlags;
        }
    }

    GIDescriptorSetLayoutVk::~GIDescriptorSetLayoutVk()
    {
        if (IsValid())
        {
            vkDestroyDescriptorSetLayout(*mDevice, mDescriptorSetLayout, nullptr);
            mDescriptorSetLayout = VK_NULL_HANDLE;
        }
    }

    bool GIDescriptorSetLayoutVk::IsValid() const
    {
        return mDescriptorSetLayout != VK_NULL_HANDLE;
    }

    void GIDescriptorSetLayoutVk::SetDebugName(const char* name) const
    {
        SetDebugNameInternal(
            mDescriptorSetLayout,
            VK_DEBUG_REPORT_OBJECT_TYPE_DESCRIPTOR_SET_LAYOUT_EXT,
            name);
    }

    void GIDescriptorSetLayoutVk::SetDebugTag(const DebugTag& tag) const
    {
        SetDebugTagInternal(
            mDescriptorSetLayout,
            VK_DEBUG_REPORT_OBJECT_TYPE_DESCRIPTOR_SET_LAYOUT_EXT,
            tag);
    }

    GIDescriptorSetLayoutVk::operator const VkDescriptorSetLayout& () const
    {
        return mDescriptorSetLayout;
    }

    bool GIDescriptorSetLayoutVk::IsPushDescriptorSet() const
    {
        return mIsPushDescriptorSet;
    }

    std::optional<VkDescriptorType> GIDescriptorSetLayoutVk::GetDescriptorType(uint32_t setId) const
    {
        auto it = mDescriptorTypes.find(setId);
        if (it == mDescriptorTypes.end())
            return std::nullopt;
        else
            return it->second;
    }

    std::optional<VkShaderStageFlags> GIDescriptorSetLayoutVk::GetStageFlags(uint32_t setId) const
    {
        auto it = mShaderStages.find(setId);
        if (it == mShaderStages.end())
            return std::nullopt;
        else
            return it->second;
    }
}