#include "GIResourceStateVk.h"
#include "GIResourceVk.h"
#include "GIBufferVk.h"
#include "GIImageVk.h"
#include "GICommandQueueVk.h"
#include "GICommandPoolVk.h"
#include "GICommandBufferVk.h"
#include "GIDeviceVk.h"

namespace AutoCAD::Graphics::Engine
{
    GIResourceStateInfoVk::GIResourceStateInfoVk(EResourceState type)
        : mState(type)
    {
        switch (mState)
        {
        default:
            assert(false);
        }
    }

    bool GIResourceStateInfoVk::IsValid() const
    {
        return mState != EResourceState::Undefined;
    }

    EResourceState GIResourceStateInfoVk::GetType() const
    {
        return mState;
    }

    VkAccessFlagBits GIResourceStateInfoVk::GetAccessFlags() const
    {
        return mAccessFlags;
    }

    VkShaderStageFlagBits GIResourceStateInfoVk::GetShaderStageFlags() const
    {
        return mShaderStageFlags;
    }

    std::optional<VkImageLayout> GIResourceStateInfoVk::GetImageLayout() const
    {
        return mImageLayout;
    }

    bool GIResourceStateInfoVk::operator==(const GIResourceStateInfoVk& rhs) const
    {
        return mState == rhs.mState;
    }

    bool GIResourceStateInfoVk::operator!=(const GIResourceStateInfoVk& rhs) const
    {
        return !(*this == rhs);
    }

    GIResourceStateVk::GIResourceStateVk(const GIResourceStateInfoVk& info)
        : mGlobalStateInfo(info)
    {}

    std::optional<GIResourceStateInfoVk> GIResourceStateVk::GetStateInfo() const
    {
        return mGlobalStateInfo;
    }

    std::optional<GIResourceStateInfoVk> GIResourceStateVk::GetSubresourceStateInfo(const VkImageSubresourceRange& subresource) const
    {
        auto it = mSubresourceStateInfos.find(subresource);
        if (it == mSubresourceStateInfos.end())
            return std::nullopt;
        else
            return it->second;
    }

    void GIResourceStateVk::SetState(const GIResourceStateInfoVk& newStateInfo)
    {
        mGlobalStateInfo = newStateInfo;
    }

    void GIResourceStateVk::SetSubresourceState(const VkImageSubresourceRange& subresource, const GIResourceStateInfoVk& newStateInfo)
    {
        mGlobalStateInfo = std::nullopt;
        mSubresourceStateInfos[subresource] = newStateInfo;
    }
}   