#pragma once

#include "GIResourceVk.h"

namespace AutoCAD::Graphics::Engine
{
    class GISamplerVk;
    class GITextureViewVk;

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

        operator const VkImage& () const;
        VkDeviceSize GetSizeInBytes() const;
        VkDeviceMemory GetDeviceMemory() const;
        VkImageUsageFlags GetUsages() const;

    protected:
        GITextureVk(); /* [1] Create image, create device memory required and bind them together */
        
        GITextureVk(); /* [2] Configure with image and memory objects precreated by VMA */

    protected:
        VkImage mImageHandle = VK_NULL_HANDLE;
        VkDeviceMemory mMemoryHandle = VK_NULL_HANDLE;
        VkDeviceSize mDeviceMemorySize = 0;
        VkImageUsageFlags mImageUsages = 0;
        
        VkImageLayout mImageLayout = VK_IMAGE_LAYOUT_UNDEFINED;
        VkFormat mFormat = VK_FORMAT_UNDEFINED;
        VkExtent3D mExtent = { 0, 0, 0 };
        VkSampleCountFlags mSampleCount = VK_SAMPLE_COUNT_1_BIT;
        VkImageUsageFlags mUsages = 0;
        uint32_t mMipLevels = 0;
        uint32_t mArrayLayers = 0;

        std::unordered_map<CACHE_INDEX, WeakPtr<GITextureViewVk>> mTextureViews;
        std::function<void()> mOnDestroyCallback;
        bool mIsMapped = false;
        SharedPtr<GISamplerVk> mSampler;
        GIResourceStateVk mResourceState;
    };

    class GITextureBuilderVk
    {
    public:
        GITextureBuilderVk(SharedPtr<GIDeviceVk> device);
        SharedPtr<GITextureVk> Build();

    private:
        SharedPtr<GIDeviceVk> mDevice;
        VkImageCreateInfo mCreateInfo = {};
    };

    class GITextureBuilderVMA
    {
    public:
        // TODO
    };
}