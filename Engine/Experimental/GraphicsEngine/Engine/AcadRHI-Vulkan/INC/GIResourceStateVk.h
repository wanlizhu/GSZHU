#pragma once

#include "GICommonVk.h"

namespace AutoCAD::Graphics::Engine
{
    enum class EResourceState
    {
        Undefined = 0,
        PreInitialized,
        General,
        VertexBuffer,
        IndexBuffer,
        UniformBuffer,
        StorageBuffer,
        InputAttachment,
        ColorAttachment,
        DepthStencilAttachment_RW,
        DepthStencilAttachment_RO,
        UniformImage,
        StorageImage,
        TransferSrc,
        TransferDst,
        ResolveSrc,
        ResolveDst,
        Present,
        StreamOut,
        IndirectArg,
        Predication,
        AccelerationStructure,
    };

    class GICommandBufferVk;

    class GIResourceStateInfoVk
    {
    public:
        GIResourceStateInfoVk();
        GIResourceStateInfoVk(EResourceState type);

        bool IsValid() const;
        EResourceState GetType() const;
        VkAccessFlagBits GetAccessFlags() const;
        std::optional<VkImageLayout> GetImageLayout() const;

        bool operator==(const GIResourceStateInfoVk& rhs) const;
        bool operator!=(const GIResourceStateInfoVk& rhs) const;
        
    private:
        EResourceState mState = EResourceState::Undefined;
        VkAccessFlagBits mAccessFlags = VK_ACCESS_FLAG_BITS_MAX_ENUM;
        std::optional<VkImageLayout> mImageLayout;
    };

    class GIResourceStateVk
    {
    public:
        GIResourceStateVk();
        GIResourceStateVk(const GIResourceStateInfoVk& info);

        std::optional<GIResourceStateInfoVk> GetGlobalState() const;
        std::optional<GIResourceStateInfoVk> GetBufferRangeState(size_t offset, size_t size) const;
        std::optional<GIResourceStateInfoVk> GetSubresourceState(const VkImageSubresourceRange& subresource) const;

        void TransitionGlobalState(const GIResourceStateInfoVk& newState, SharedPtr<GICommandBufferVk> cmdbuf = nullptr);
        void TransitionBufferRangeState(size_t offset, size_t size, const GIResourceStateInfoVk& newState, SharedPtr<GICommandBufferVk> cmdbuf = nullptr);
        void TransitionSubresourceState(const VkImageSubresourceRange& subresource, const GIResourceStateInfoVk& newState, SharedPtr<GICommandBufferVk> cmdbuf = nullptr);

    private:
        static CACHE_INDEX ComputeCacheIndex(size_t offset, size_t size);
        static CACHE_INDEX ComputeCacheIndex(const VkImageSubresourceRange& subresource);

    private:
        std::optional<GIResourceStateInfoVk> mGlobalStateInfo;
        std::unordered_map<CACHE_INDEX, GIResourceStateInfoVk> mBufferRangeStateInfos;
        std::unordered_map<CACHE_INDEX, GIResourceStateInfoVk> mSubresourceStateInfos;
    };
}