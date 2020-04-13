#pragma once

#include "GIResourceVk.h"

namespace AutoCAD::Graphics::Engine
{
    class GIImageViewVk;
    class GICommandBufferVk;

    class GIImageVk : public GIResourceVk
    {
        DECL_DEVICE_OBJECT(GIImageVk)
    public:
        static SharedPtr<GIImageVk> Create(
            SharedPtr<GIDeviceVk> device,
            VkImageType imageType,
            VkFormat format,
            VkExtent3D extent,
            VkImageLayout imageLayout,
            VkSampleCountFlagBits samples,
            VkImageUsageFlagBits usage,
            std::vector<uint32_t> sharingQueues,
            VkMemoryPropertyFlags properties,
            const void* initialData,
            EResourceState initialState
        );

        virtual ~GIImageVk();
        virtual bool IsValid() const override;
        virtual void SetDebugName(const char* name) const override;
        virtual void SetDebugTag(const DebugTag& tag) const override;
        virtual EResourceType GetResourceType() const override;
        virtual EResourceState GetResourceState() const override;
        virtual void TransitionState(const EResourceState& newState) override;
        virtual void SetOnDestroyCallback(const std::function<void()>& func) override;

        operator const VkImage& () const;
        VkDeviceMemory GetMemory() const;
        VkMemoryPropertyFlags GetMemoryProperties() const;
        VkDeviceSize GetSizeInBytes() const;
        VkImageUsageFlags GetUsage() const;
        VkImageType GetImageType() const;
        VkFormat GetFormat() const;
        VkExtent3D GetExtent() const;
        uint32_t GetMipLevels() const;
        uint32_t GetArrayLayers() const;
        VkSampleCountFlagBits GetSampleCount() const;
        bool HasGlobalState() const;
        EResourceState GetSubresourceState(const VkImageSubresourceRange& subresource) const;
        void TransitionSubresourceState(const VkImageSubresourceRange& subresource, const EResourceState& newState);
        SharedPtr<GIImageViewVk> GetTextureView(
            VkImageViewType viewType,
            VkFormat format,
            const VkImageSubresourceRange& subresource
        );

        void CreateMipmaps(uint32_t mipLevels);
        void* Map(size_t offset = 0, size_t size = VK_WHOLE_SIZE);
        void Unmap();
        void UpdateData(
            size_t offset,
            size_t size,
            const void* data,
            SharedPtr<GICommandBufferVk> cmdbuf
        );

    protected:
        GIImageVk(SharedPtr<GIDeviceVk> device);
        
    protected:
        VkImage mImageHandle = VK_NULL_HANDLE;
        VkDeviceMemory mMemoryHandle = VK_NULL_HANDLE;
        VkMemoryPropertyFlags mMemoryProperties = 0;
        VkImageType mImageType = VK_IMAGE_TYPE_2D;
        VkFormat mFormat = VK_FORMAT_UNDEFINED;
        VkExtent3D mExtent = { 0, 0, 0 };
        VkDeviceSize mSizeInBytes = 0;
        uint32_t mMipLevels = 1;
        uint32_t mArrayLayers = 1;
        VkSampleCountFlagBits mSampleCount = VK_SAMPLE_COUNT_1_BIT;
        VkImageUsageFlags mUsage = VK_IMAGE_USAGE_SAMPLED_BIT;

        std::unordered_map<CACHE_INDEX, WeakPtr<GIImageViewVk>> mTextureViews;
        std::function<void()> mOnDestroyCallback;
        EResourceState mGlobalResourceState;
        std::unordered_map<VkImageSubresourceRange, EResourceState> mSubresourceStates;
        bool mIsMapped = false;
    };
}