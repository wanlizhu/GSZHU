#pragma once

#include "GIResourceVk.h"

namespace AutoCAD::Graphics::Engine
{
    class GISamplerVk;
    class GITextureViewVk;

    struct GITextureInfoVk
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

    class GITextureVk : public GIResourceVk
    {
        friend class GITextureBuilderVMA;
        friend class GITextureBuilderVk;
        DECL_DEVICE_OBJECT(GITextureVk)
    public:
        virtual ~GITextureVk();
        virtual bool IsValid() const override;
        virtual void SetDebugName(const char* name) const override;
        virtual void SetDebugTag(const DebugTag& tag) const override;
        virtual EResourceType GetResourceType() const override;
        virtual GIResourceStateVk& GetResourceState() override final;
        virtual void TransitionState(const GIResourceStateVk& newState) override final;
        void TransitionSubresourceState(const VkImageSubresourceRange& subresource, const GIResourceStateVk& newState);

        operator const VkImage& () const;
        GITextureInfoVk const& GetInfo() const;
        SharedPtr<GISamplerVk> GetSampler() const;
        SharedPtr<GITextureViewVk> GetTextureView(
            VkImageViewType viewType,
            VkFormat format,
            const VkImageSubresourceRange& subresource = {}
        );

        void SetSampler(SharedPtr<GISamplerVk> sampler);
        void SetOnDestroyCallback(const std::function<void()> &func);
        void CreateMipmaps(uint32_t mipLevels);

    protected:
        GITextureVk(
            SharedPtr<GIDeviceVk> device, 
            const VkImageCreateInfo& createInfo,
            const void* data,
            VkMemoryPropertyFlags properties,
            EResourceState initialState
        ); /* [1] Create image, create device memory required and bind them together */
        
        GITextureVk(
            SharedPtr<GIDeviceVk> device,
            const GITextureInfoVk& info,
            EResourceState initialState
        ); /* [2] Configure with image and memory objects precreated by VMA */

    protected:
        VkImage mImageHandle = VK_NULL_HANDLE;
        VkDeviceMemory mMemoryHandle = VK_NULL_HANDLE;
        GITextureInfoVk mTextureInfo;

        std::unordered_map<CACHE_INDEX, WeakPtr<GITextureViewVk>> mTextureViews;
        std::function<void()> mOnDestroyCallback;
        GIResourceStateVk mResourceState;
        bool mIsMapped = false;
        SharedPtr<GISamplerVk> mSampler;
    };
}