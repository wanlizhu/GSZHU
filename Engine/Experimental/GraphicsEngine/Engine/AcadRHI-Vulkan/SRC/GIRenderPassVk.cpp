#include "GIRenderPassVk.h"
#include "GIDeviceVk.h"

namespace AutoCAD::Graphics::Engine
{
    GIRenderPassVk::~GIRenderPassVk()
    {
        if (IsValid())
        {
            vkDestroyRenderPass(*mDevice, mRenderPassHandle, nullptr);
            mRenderPassHandle = VK_NULL_HANDLE;
        }
    }

    bool GIRenderPassVk::IsValid() const
    {
        return mRenderPassHandle != VK_NULL_HANDLE;
    }

    void GIRenderPassVk::SetDebugName(const char* name) const
    {
        SetDebugNameInternal(
            mRenderPassHandle,
            VK_DEBUG_REPORT_OBJECT_TYPE_RENDER_PASS_EXT,
            name
        );
    }

    void GIRenderPassVk::SetDebugTag(const DebugTag& tag) const
    {
        SetDebugTagInternal(
            mRenderPassHandle,
            VK_DEBUG_REPORT_OBJECT_TYPE_RENDER_PASS_EXT,
            tag
        );
    }

    GIRenderPassVk::operator const VkRenderPass& () const
    {
        return mRenderPassHandle;
    }

    uint32_t GIRenderPassVk::GetSampleCount() const
    {
        return mSampleCount;
    }

    uint32_t GIRenderPassVk::GetColorCount() const
    {
        return (uint32_t)mRenderTargetFormats.size();
    }

    VkFormat GIRenderPassVk::GetColorFormat(uint32_t index) const
    {
        assert(index < GetColorCount());
        return mRenderTargetFormats[index];
    }

    VkFormat GIRenderPassVk::GetDepthStencilFormat() const
    {
        return mDepthStencilFormat;
    }

    GIRenderPassVk::GIRenderPassVk(
        SharedPtr<GIDeviceVk> device,
        const VkRenderPassCreateInfo& createInfo
    )
        : GIDeviceObjectVk(device)
    {}
}