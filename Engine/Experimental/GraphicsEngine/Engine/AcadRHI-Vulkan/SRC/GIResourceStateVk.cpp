#include "GIResourceStateVk.h"
#include "GICommandQueueVk.h"
#include "GICommandPoolVk.h"
#include "GICommandBufferVk.h"
#include "GIDeviceVk.h"

namespace AutoCAD::Graphics::Engine
{
    GIResourceStateInfoVk::GIResourceStateInfoVk()
        : mState(EResourceState::Undefined)
    {}

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

    GIResourceStateVk::GIResourceStateVk()
        : mGlobalStateInfo(GIResourceStateInfoVk(EResourceState::Undefined))
    {}

    GIResourceStateVk::GIResourceStateVk(const GIResourceStateInfoVk& info)
        : mGlobalStateInfo(info)
    {}

    std::optional<GIResourceStateInfoVk> GIResourceStateVk::GetGlobalState() const
    {
        return mGlobalStateInfo;
    }

    std::optional<GIResourceStateInfoVk> GIResourceStateVk::GetBufferRangeState(size_t offset, size_t size) const
    {
        auto it = mBufferRangeStateInfos.find(ComputeCacheIndex(offset, size));
        if (it == mBufferRangeStateInfos.end())
            return std::nullopt;
        else
            return it->second;
    }

    std::optional<GIResourceStateInfoVk> GIResourceStateVk::GetSubresourceState(const VkImageSubresourceRange& subresource) const
    {
        auto it = mSubresourceStateInfos.find(ComputeCacheIndex(subresource));
        if (it == mSubresourceStateInfos.end())
            return std::nullopt;
        else
            return it->second;
    }

    void GIResourceStateVk::TransitionGlobalState(const GIResourceStateInfoVk& newState, SharedPtr<GICommandBufferVk> cmdbuf)
    {}

    void GIResourceStateVk::TransitionBufferRangeState(size_t offset, size_t size, const GIResourceStateInfoVk& newState, SharedPtr<GICommandBufferVk> cmdbuf)
    {}

    void GIResourceStateVk::TransitionSubresourceState(const VkImageSubresourceRange& subresource, const GIResourceStateInfoVk& newState, SharedPtr<GICommandBufferVk> cmdbuf)
    {}

    CACHE_INDEX GIResourceStateVk::ComputeCacheIndex(size_t offset, size_t size)
    {
        return std::hash<size_t>()(offset) ^ std::hash<size_t>()(size);
    }

    CACHE_INDEX GIResourceStateVk::ComputeCacheIndex(const VkImageSubresourceRange& subresource)
    {
        return std::hash<uint32_t>()(subresource.aspectMask) 
            ^ std::hash<uint32_t>()(subresource.baseArrayLayer)
            ^ std::hash<uint32_t>()(subresource.baseMipLevel)
            ^ std::hash<uint32_t>()(subresource.layerCount)
            ^ std::hash<uint32_t>()(subresource.levelCount);
    }
}   