#include "GISamplerVk.h"
#include "GIDeviceVk.h"

namespace AutoCAD::Graphics::Engine
{
    GISamplerVk::GISamplerVk(
        SharedPtr<GIDeviceVk> device,
        const VkSamplerCreateInfo& createInfo
    )
        : GIDeviceObjectVk(device)
        , mSamplerInfo(createInfo)
    {}

    GISamplerVk::~GISamplerVk()
    {
        if (IsValid())
        {
            vkDestroySampler(*mDevice, mSamplerHandle, nullptr);
            mSamplerHandle = VK_NULL_HANDLE;
        }
    }

    bool GISamplerVk::IsValid() const
    {
        return mSamplerHandle != VK_NULL_HANDLE;
    }

    void GISamplerVk::SetDebugName(const char* name) const
    {
        SetDebugNameInternal(
            mSamplerHandle,
            VK_DEBUG_REPORT_OBJECT_TYPE_SAMPLER_EXT,
            name
        );
    }

    void GISamplerVk::SetDebugTag(const DebugTag& tag) const
    {
        SetDebugTagInternal(
            mSamplerHandle,
            VK_DEBUG_REPORT_OBJECT_TYPE_SAMPLER_EXT,
            tag
        );
    }

    GISamplerVk::operator const VkSampler& () const
    {
        return mSamplerHandle;
    }

    VkSamplerCreateInfo const& GISamplerVk::GetSamplerInfo() const
    {
        return mSamplerInfo;
    }
}