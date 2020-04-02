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

    class GIResourceVk;
    class GICommandBufferVk;

    class GIResourceStateInfoVk
    {
    public:
        GIResourceStateInfoVk();
        GIResourceStateInfoVk(EResourceState type);

        bool IsValid() const;
        EResourceState GetType() const;
        VkAccessFlagBits GetAccessFlags() const;
        VkShaderStageFlagBits GetShaderStageFlags() const;
        std::optional<VkImageLayout> GetImageLayout() const;

        bool operator==(const GIResourceStateInfoVk& rhs) const;
        bool operator!=(const GIResourceStateInfoVk& rhs) const;
        
    private:
        EResourceState mState = EResourceState::Undefined;
        VkShaderStageFlagBits mShaderStageFlags = VK_SHADER_STAGE_ALL;
        VkAccessFlagBits mAccessFlags = VK_ACCESS_FLAG_BITS_MAX_ENUM;
        std::optional<VkImageLayout> mImageLayout;
    };

    class GIResourceStateVk
    {
    public:
        GIResourceStateVk(WeakPtr<GIResourceVk> resource);
        GIResourceStateVk(WeakPtr<GIResourceVk> resource, const GIResourceStateInfoVk& info);

        std::optional<GIResourceStateInfoVk> GetStateInfo() const;
        std::optional<GIResourceStateInfoVk> GetSubresourceStateInfo(const VkImageSubresourceRange& subresource) const;

        void TransitionState(const GIResourceStateInfoVk& newStateInfo, SharedPtr<GICommandBufferVk> cmdbuf = nullptr);
        void TransitionSubresourceState(const VkImageSubresourceRange& subresource, const GIResourceStateInfoVk& newStateInfo, SharedPtr<GICommandBufferVk> cmdbuf = nullptr);

    private:
        static CACHE_INDEX ComputeCacheIndex(const VkImageSubresourceRange& subresource);

    private:
        WeakPtr<GIResourceVk> mResource;
        std::optional<GIResourceStateInfoVk> mGlobalStateInfo;
        std::unordered_map<CACHE_INDEX, GIResourceStateInfoVk> mSubresourceStateInfos;
    };
}