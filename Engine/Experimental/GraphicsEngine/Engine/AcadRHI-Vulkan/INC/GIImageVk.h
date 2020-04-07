#pragma once

#include "GIResourceVk.h"

namespace AutoCAD::Graphics::Engine
{
    class GISamplerVk;
    class GIImageViewVk;
    class GIDeviceMemoryAllocatorVk;

    struct GIImageInfoVk
    {
        VkImageType imageType = VK_IMAGE_TYPE_2D;
        VkFormat format = VK_FORMAT_UNDEFINED;
        VkExtent3D extent = { 0, 0, 0 };
        uint32_t mipLevels = 1;
        uint32_t arrayLayers = 1;
        VkSampleCountFlagBits samples = VK_SAMPLE_COUNT_1_BIT;
        VkImageTiling tiling = VK_IMAGE_TILING_OPTIMAL;
        VkImageUsageFlags usage = VK_IMAGE_USAGE_SAMPLED_BIT;
        VkSharingMode sharingMode = VK_SHARING_MODE_EXCLUSIVE;
        VkImageLayout imageLayout = VK_IMAGE_LAYOUT_GENERAL;
        VkDeviceSize sizeInBytes = 0;
    };

    class GIImageVk : public GIResourceVk
    {
        friend class GIImageBuilderVMA;
        friend class GIImageBuilderVk;
        DECL_DEVICE_OBJECT(GIImageVk)
    public:
        virtual ~GIImageVk();
        virtual bool IsValid() const override;
        virtual void SetDebugName(const char* name) const override;
        virtual void SetDebugTag(const DebugTag& tag) const override;
        virtual EResourceType GetResourceType() const override;
        virtual GIResourceStateVk& GetResourceState() override final;
        virtual void TransitionState(const GIResourceStateVk& newState) override final;
        void TransitionSubresourceState(const VkImageSubresourceRange& subresource, const GIResourceStateVk& newState);

        operator const VkImage& () const;
        GIImageInfoVk const& GetInfo() const;
        SharedPtr<GIImageViewVk> GetTextureView(
            VkImageViewType viewType,
            VkFormat format,
            const VkImageSubresourceRange& subresource = {}
        );

        void SetOnDestroyCallback(const std::function<void()> &func);
        void CreateMipmaps(uint32_t mipLevels);

    protected:
        GIImageVk(
            SharedPtr<GIDeviceVk> device, 
            const VkImageCreateInfo& createInfo,
            const void* data,
            VkMemoryPropertyFlags properties,
            EResourceState initialState
        ); /* [1] Create image, create device memory required and bind them together */
        
    protected:
        VkImage mImageHandle = VK_NULL_HANDLE;
        VkDeviceMemory mMemoryHandle = VK_NULL_HANDLE;
        GIImageInfoVk mImageInfo;

        std::unordered_map<CACHE_INDEX, WeakPtr<GIImageViewVk>> mTextureViews;
        std::function<void()> mOnDestroyCallback;
        GIResourceStateVk mResourceState;
        bool mIsMapped = false;
    };

    class GIImageBuilderVk
    {
    public:
        GIImageBuilderVk(SharedPtr<GIDeviceVk> device);
        GIImageBuilderVk& SetAllocator(SharedPtr<GIDeviceMemoryAllocatorVk> allocator);
        GIImageBuilderVk& SetImageType(VkImageType type);
        GIImageBuilderVk& SetFormat(VkFormat format);
        GIImageBuilderVk& SetExtent(VkExtent3D extent);
        GIImageBuilderVk& SetMipLevels(uint32_t mipLevels);
        GIImageBuilderVk& SetArrayLayers(uint32_t arrayLevels);
        GIImageBuilderVk& SetSamples(VkSampleCountFlagBits samples);
        GIImageBuilderVk& SetImageTiling(VkImageTiling tiling);
        GIImageBuilderVk& AddImageUsages(VkImageUsageFlags usages);
        GIImageBuilderVk& AddMemoryProperties(VkMemoryPropertyFlags properties);
        GIImageBuilderVk& AddSharedQueue(uint32_t queue);
        GIImageBuilderVk& SetInitialLayout(VkImageLayout layout);
        GIImageBuilderVk& SetInitialData(const void* data);
        GIImageBuilderVk& SetInitialState(EResourceState state);
        SharedPtr<GIImageVk> Build();

    private:
        SharedPtr<GIDeviceVk> mDevice;
        VkImageCreateInfo mCreateInfo = {};
        VkMemoryPropertyFlags mProperties = 0;
        SharedPtr<GIDeviceMemoryAllocatorVk> mAllocator;

        const void* mInitialData = nullptr;
        EResourceState mInitialState = EResourceState::Undefined;
        std::vector<uint32_t> mSharedQueues;
    };
}