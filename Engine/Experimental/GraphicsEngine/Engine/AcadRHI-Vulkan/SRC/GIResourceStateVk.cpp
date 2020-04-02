#include "GIResourceStateVk.h"
#include "GIResourceVk.h"
#include "GIBufferVk.h"
#include "GITextureVk.h"
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

    GIResourceStateVk::GIResourceStateVk(WeakPtr<GIResourceVk> resource)
        : mResource(resource)
        , mGlobalStateInfo(GIResourceStateInfoVk(EResourceState::Undefined))
    {}

    GIResourceStateVk::GIResourceStateVk(WeakPtr<GIResourceVk> resource, const GIResourceStateInfoVk& info)
        : mResource(resource)
        , mGlobalStateInfo(info)
    {}

    std::optional<GIResourceStateInfoVk> GIResourceStateVk::GetStateInfo() const
    {
        return mGlobalStateInfo;
    }

    std::optional<GIResourceStateInfoVk> GIResourceStateVk::GetSubresourceStateInfo(const VkImageSubresourceRange& subresource) const
    {
        auto it = mSubresourceStateInfos.find(ComputeCacheIndex(subresource));
        if (it == mSubresourceStateInfos.end())
            return std::nullopt;
        else
            return it->second;
    }

    void GIResourceStateVk::TransitionState(const GIResourceStateInfoVk& newStateInfo, SharedPtr<GICommandBufferVk> cmdbuf)
    {

        auto resource = mResource.lock();
        if (!resource ||
            !mGlobalStateInfo.has_value() ||
            mGlobalStateInfo == newStateInfo)
        {
            LOG_WARNING("Failed to transition resource state.\n");
            return;
        }

        if (resource->GetResourceType() == EResourceType::Buffer)
        {
            SharedPtr<GIBufferVk> buffer = std::dynamic_pointer_cast<GIBufferVk>(resource);
            VkShaderStageFlagBits srcShaderStage = mGlobalStateInfo.value().GetShaderStageFlags();
            VkShaderStageFlagBits dstShaderStage = newStateInfo.GetShaderStageFlags();

            VkBufferMemoryBarrier barrier = {};
            barrier.sType = VK_STRUCTURE_TYPE_BUFFER_MEMORY_BARRIER;
            barrier.srcAccessMask = mGlobalStateInfo.value().GetAccessFlags();
            barrier.dstAccessMask = newStateInfo.GetAccessFlags();
            barrier.buffer = *buffer;
            barrier.offset = 0;
            barrier.size = VK_WHOLE_SIZE;

            vkCmdPipelineBarrier(*cmdbuf, srcShaderStage, dstShaderStage, 0, 0, nullptr, 1, &barrier, 0, nullptr);
            mGlobalStateInfo = newStateInfo;
        }
        else
        {

        }
    }

    void GIResourceStateVk::TransitionSubresourceState(const VkImageSubresourceRange& subresource, const GIResourceStateInfoVk& newState, SharedPtr<GICommandBufferVk> cmdbuf)
    {}

    CACHE_INDEX GIResourceStateVk::ComputeCacheIndex(const VkImageSubresourceRange& subresource)
    {
        return std::hash<uint32_t>()(subresource.aspectMask) 
            ^ std::hash<uint32_t>()(subresource.baseArrayLayer)
            ^ std::hash<uint32_t>()(subresource.baseMipLevel)
            ^ std::hash<uint32_t>()(subresource.layerCount)
            ^ std::hash<uint32_t>()(subresource.levelCount);
    }
}   