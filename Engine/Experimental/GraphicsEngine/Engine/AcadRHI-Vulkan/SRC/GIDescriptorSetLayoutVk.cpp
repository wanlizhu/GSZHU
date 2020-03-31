#include "GIDescriptorSetLayoutVk.h"
#include "GIShaderReflectionVk.h"
#include "GIDeviceVk.h"

namespace AutoCAD::Graphics::Engine
{
    SharedPtr<GIDescriptorSetLayoutVk> GIDescriptorSetLayoutVk::Create(
        SharedPtr<GIDeviceVk> device,
        SharedPtr<GIShaderReflectionVk> reflection,
        const std::vector<VkDescriptorSetLayoutBinding>& setBindings,
        bool isPushDescriptorSet
    )
    {
        return SharedPtr<GIDescriptorSetLayoutVk>(new GIDescriptorSetLayoutVk(device, reflection, setBindings, isPushDescriptorSet));
    }

    GIDescriptorSetLayoutVk::GIDescriptorSetLayoutVk(
        SharedPtr<GIDeviceVk> device,
        SharedPtr<GIShaderReflectionVk> reflection,
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
        VK_CHECK(vkCreateDescriptorSetLayout(*device, &createInfo, nullptr, &mDescriptorSetLayoutHandle));

        for (const auto& binding : setBindings)
        {
            mBindings[binding.binding] = binding;
        }
    }

    GIDescriptorSetLayoutVk::~GIDescriptorSetLayoutVk()
    {
        if (IsValid())
        {
            vkDestroyDescriptorSetLayout(*mDevice, mDescriptorSetLayoutHandle, nullptr);
            mDescriptorSetLayoutHandle = VK_NULL_HANDLE;
        }
    }

    bool GIDescriptorSetLayoutVk::IsValid() const
    {
        return mDescriptorSetLayoutHandle != VK_NULL_HANDLE;
    }

    void GIDescriptorSetLayoutVk::SetDebugName(const char* name) const
    {
        SetDebugNameInternal(
            mDescriptorSetLayoutHandle,
            VK_DEBUG_REPORT_OBJECT_TYPE_DESCRIPTOR_SET_LAYOUT_EXT,
            name
        );
    }

    void GIDescriptorSetLayoutVk::SetDebugTag(const DebugTag& tag) const
    {
        SetDebugTagInternal(
            mDescriptorSetLayoutHandle,
            VK_DEBUG_REPORT_OBJECT_TYPE_DESCRIPTOR_SET_LAYOUT_EXT,
            tag
        );
    }

    GIDescriptorSetLayoutVk::operator const VkDescriptorSetLayout& () const
    {
        return mDescriptorSetLayoutHandle;
    }

    bool GIDescriptorSetLayoutVk::IsPushDescriptorSet() const
    {
        return mIsPushDescriptorSet;
    }

    std::unordered_map<BINDING_ID, VkDescriptorSetLayoutBinding> const& GIDescriptorSetLayoutVk::GetBindings() const
    {
        return mBindings;
    }

    std::optional<VkDescriptorSetLayoutBinding> GIDescriptorSetLayoutVk::GetBindingByName(const char* name) const
    {
        auto var = mShaderReflection->GetVariable(name);

        if (var.has_value() && var.value().GetBinding().has_value())
        {
            BINDING_ID bindingId = var.value().GetBinding().value().GetBindingId();
            auto it = mBindings.find(bindingId);
            if (it == mBindings.end())
                return std::nullopt;
            else
                return it->second;
        }

        return std::nullopt;
    }
}