#include "GIPipelineLayoutVk.h"
#include "GIDescriptorSetLayoutVk.h"
#include "GIDeviceVk.h"

namespace AutoCAD::Graphics::Engine
{
    SharedPtr<GIPipelineLayoutVk> GIPipelineLayoutVk::Create(
        SharedPtr<GIDeviceVk> device,
        const std::vector<SharedPtr<GIDescriptorSetLayoutVk>>& setLayouts,
        const std::vector<VkPushConstantRange>& pushConstantRanges)
    {
        return SharedPtr<GIPipelineLayoutVk>(new GIPipelineLayoutVk(device, setLayouts, pushConstantRanges));
    }

    GIPipelineLayoutVk::GIPipelineLayoutVk(
        SharedPtr<GIDeviceVk> device,
        const std::vector<SharedPtr<GIDescriptorSetLayoutVk>>& setLayouts,
        const std::vector<VkPushConstantRange>& pushConstantRanges
    )
        : GIDeviceObjectVk(device)
    {
        
    }


    GIPipelineLayoutVk::~GIPipelineLayoutVk()
    {
        if (IsValid())
        {
            vkDestroyPipelineLayout(*mDevice, mPipelineLayout, nullptr);
            mPipelineLayout = VK_NULL_HANDLE;
        }
    }

    bool GIPipelineLayoutVk::IsValid() const
    {
        return mPipelineLayout != VK_NULL_HANDLE;
    }

    void GIPipelineLayoutVk::SetDebugName(const char* name) const
    {
        SetDebugNameInternal(
            mPipelineLayout,
            VK_DEBUG_REPORT_OBJECT_TYPE_PIPELINE_LAYOUT_EXT,
            name);
    }

    void GIPipelineLayoutVk::SetDebugTag(const DebugTag& tag) const
    {
        SetDebugTagInternal(
            mPipelineLayout,
            VK_DEBUG_REPORT_OBJECT_TYPE_PIPELINE_LAYOUT_EXT,
            tag);
    }

    GIPipelineLayoutVk::operator const VkPipelineLayout& () const
    {
        return mPipelineLayout;
    }

    std::unordered_map<uint32_t, SharedPtr<GIDescriptorSetLayoutVk>> const& GIPipelineLayoutVk::GetDescriptorSetLayouts() const
    {
        return mDescriptorSetLayouts;
    }

    std::optional<SharedPtr<GIDescriptorSetLayoutVk>> GIPipelineLayoutVk::GetDescriptorSetLayout(uint32_t setId) const
    {
        auto it = mDescriptorSetLayouts.find(setId);
        if (it == mDescriptorSetLayouts.end())
            return nullptr;
        else
            return it->second;
    }

    std::optional<VkPushConstantRange> GIPipelineLayoutVk::GetPushConstantRange(VkPipelineStageFlags stage) const
    {
        auto it = mPushConstantRanges.find(stage);
        if (it == mPushConstantRanges.end())
            return std::nullopt;
        else
            return it->second;
    }
}