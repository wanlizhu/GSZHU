#include "GIComputePipelineVk.h"
#include "GIDeviceVk.h"

namespace AutoCAD::Graphics::Engine
{
    GIComputePipelineVk::GIComputePipelineVk(
        SharedPtr<GIDeviceVk> device,
        const VkComputePipelineCreateInfo& createInfo
    )
        : GIDeviceObjectVk(device)
    {}

    GIComputePipelineVk::~GIComputePipelineVk()
    {
        if (IsValid())
        {
            vkDestroyPipeline(*mDevice, mPipeline, nullptr);
            mPipeline = VK_NULL_HANDLE;
        }
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
}