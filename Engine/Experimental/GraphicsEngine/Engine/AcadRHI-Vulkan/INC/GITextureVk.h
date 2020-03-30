#pragma once

#include "GIDeviceResourceVk.h"

namespace AutoCAD::Graphics::Engine
{
    class GISamplerVk;

    class GITextureVk : public GIDeviceResourceVk
    {
        friend class GITextureBuilderVk;
        DECL_DEVICE_OBJECT(GITextureVk)
    public:
        virtual ~GITextureVk();
        virtual bool IsValid() const override;
        virtual void SetDebugName(const char* name) const override;
        virtual void SetDebugTag(const DebugTag& tag) const override;
        virtual EResourceType GetResourceType() const override;

        operator const VkImage& () const;
        VkDeviceSize GetSizeInBytes() const;
        VkDeviceMemory GetDeviceMemory() const;
        VkBufferUsageFlags GetUsages() const;

    protected:
        GITextureVk(
            SharedPtr<GIDeviceVk> device,
            const VkImageCreateInfo& createInfo,
            const VkMemoryAllocateInfo& allocInfo,
            const void* data,
            bool useStagingBuffer
        );

    protected:
        VkImage mImage = VK_NULL_HANDLE;
        VkDeviceMemory mDeviceMemory = VK_NULL_HANDLE;
        VkDeviceSize mDeviceMemorySize = 0;
        SharedPtr<GISamplerVk> mSampler;
        bool mIsMapped = false;

        VkImageLayout mImageLayout = VK_IMAGE_LAYOUT_UNDEFINED;
        VkFormat mFormat = VK_FORMAT_UNDEFINED;
        VkExtent3D mExtent = { 0, 0, 0 };
        VkSampleCountFlags mSampleCount = VK_SAMPLE_COUNT_1_BIT;
        VkImageUsageFlags mUsages = 0;
        uint32_t mMipLevels = 0;
        uint32_t mArrayLayers = 0;

    private:
        std::optional<VkBuffer> mStagingBuffer;
        std::optional<VkDeviceMemory> mStagingBufferMemory;
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
}