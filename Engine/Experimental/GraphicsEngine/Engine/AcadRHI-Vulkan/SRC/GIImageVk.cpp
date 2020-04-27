#include "GIImageVk.h"
#include "GIImageViewVk.h"
#include "GISamplerVk.h"
#include "GIDeviceVk.h"
#include "GIDeviceMemoryAllocatorVk.h"

namespace AutoCAD::Graphics::Engine
{
    SharedPtr<GIImageVk> GIImageVk::Create(
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
    )
    {
        VkImage image = VK_NULL_HANDLE;
        VkDeviceMemory memory = VK_NULL_HANDLE;
        VkImageCreateInfo createInfo = {};
        createInfo.sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO;
        createInfo.pNext = nullptr;
        createInfo.flags = 0;
        createInfo.imageType = imageType;
        createInfo.format = format;
        createInfo.extent = extent;
        createInfo.mipLevels = 1;
        createInfo.arrayLayers = 1;
        createInfo.samples = samples;
        createInfo.tiling = VK_IMAGE_TILING_OPTIMAL;
        createInfo.usage = usage;
        createInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;
        createInfo.queueFamilyIndexCount = 0;
        createInfo.pQueueFamilyIndices = nullptr;
        createInfo.initialLayout = imageLayout;

        VK_CHECK(vkCreateImage(*device, &createInfo, nullptr, &image));

        VkMemoryRequirements requirements = {};
        VkMemoryAllocateInfo allocInfo = {};

        vkGetImageMemoryRequirements(*device, image, &requirements);
        allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
        allocInfo.pNext = nullptr;
        allocInfo.allocationSize = requirements.size;
        allocInfo.memoryTypeIndex = device->GetMemoryTypeIndex(requirements.memoryTypeBits, properties);
        VK_CHECK(vkAllocateMemory(*device, &allocInfo, nullptr, &memory));
        VK_CHECK(vkBindImageMemory(*device, image, memory, 0));

        SharedPtr<GIImageVk> result(new GIImageVk(device));
        result->mImageHandle = image;
        result->mMemoryHandle = memory;
        result->mMemoryProperties = properties;
        result->mImageType = imageType;
        result->mFormat = format;
        result->mExtent = extent;
        result->mSizeInBytes = requirements.size;
        result->mMipLevels = 1;
        result->mArrayLayers = 1;
        result->mSampleCount = samples;
        result->mUsage = usage;
        result->mGlobalResourceState = initialState;

        if (initialData != nullptr)
        {
            result->UpdateData(0, requirements.size, initialData, nullptr);
        }

        return result;
    }

    GIImageVk::GIImageVk(SharedPtr<GIDeviceVk> device)
        : GIResourceVk(device)
    {}

    GIImageVk::~GIImageVk()
    {
        if (mOnDestroyCallback)
        {
            mOnDestroyCallback();
        }

        if (IsValid())
        {
            vkDestroyImage(*mDevice, mImageHandle, nullptr);
            vkFreeMemory(*mDevice, mMemoryHandle, nullptr);

            mImageHandle = VK_NULL_HANDLE;
            mMemoryHandle = VK_NULL_HANDLE;
        }
    }

    bool GIImageVk::IsValid() const
    {
        return mImageHandle != VK_NULL_HANDLE && mMemoryHandle != VK_NULL_HANDLE;
    }

    void GIImageVk::SetDebugName(const char* name) const
    {
        SetDebugNameInternal(
            mImageHandle, 
            VK_DEBUG_REPORT_OBJECT_TYPE_IMAGE_EXT,
            name
        );
    }

    void GIImageVk::SetDebugTag(const DebugTag& tag) const
    {
        SetDebugTagInternal(
            mImageHandle,
            VK_DEBUG_REPORT_OBJECT_TYPE_IMAGE_EXT,
            tag
        );
    }

    EResourceType GIImageVk::GetResourceType() const
    {
        return EResourceType::Image;
    }

    EResourceState GIImageVk::GetResourceState() const
    {
        return mGlobalResourceState;
    }

    void GIImageVk::TransitionState(const EResourceState& newState)
    {
        //TODO
    }

    void GIImageVk::TransitionSubresourceState(const VkImageSubresourceRange& subresource, const EResourceState& newState)
    {}

    GIImageVk::operator const VkImage& () const
    {
        return mImageHandle;
    }

    VkDeviceMemory GIImageVk::GetMemory() const
    {
        return mMemoryHandle;
    }

    VkMemoryPropertyFlags GIImageVk::GetMemoryProperties() const
    {
        return mMemoryProperties;
    }

    VkDeviceSize GIImageVk::GetSizeInBytes() const
    {
        return mSizeInBytes;
    }

    VkImageUsageFlags GIImageVk::GetUsage() const
    {
        return mUsage;
    }

    VkImageType GIImageVk::GetImageType() const
    {
        return mImageType;
    }

    VkFormat GIImageVk::GetFormat() const
    {
        return mFormat;
    }

    VkExtent3D GIImageVk::GetExtent() const;
    uint32_t GIImageVk::GetMipLevels() const;
    uint32_t GIImageVk::GetArrayLayers() const;
    VkSampleCountFlagBits GIImageVk::GetSampleCount() const;
    bool GIImageVk::HasGlobalState() const;
    EResourceState GIImageVk::GetSubresourceState(const VkImageSubresourceRange& subresource) const;

    SharedPtr<GIImageViewVk> GIImageVk::GetTextureView(
        VkImageViewType viewType,
        VkFormat format,
        const VkImageSubresourceRange& subresource
    )
    {
        auto key = GIImageViewVk::ComputeCacheIndex(viewType, format, subresource);
        auto view = mTextureViews.find(key);
        if (view == mTextureViews.end())
        {
            auto newView = GIImageViewVk::Create(shared_from_this(), viewType, format, subresource);
            mTextureViews[key] = newView;
            return newView;
        }
        else
        {
            return view->second.lock();
        }
    }

    void GIImageVk::SetOnDestroyCallback(const std::function<void()>& func)
    {
        mOnDestroyCallback = func;
    }

    void GIImageVk::CreateMipmaps(uint32_t mipLevels)
    {}
}