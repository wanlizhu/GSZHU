#include "GIImageViewVk.h"
#include "GIImageVk.h"
#include "GIDeviceVk.h"

namespace AutoCAD::Graphics::Engine
{
    SharedPtr<GIImageViewVk> GIImageViewVk::Create(
        SharedPtr<GIImageVk> image,
        VkImageViewType type,
        VkFormat format,
        const VkImageSubresourceRange& subresource
    )
    {
        auto device = image->GetDevice();

        VkImageViewCreateInfo createInfo = {};
        VkImageView imageView = VK_NULL_HANDLE;
        createInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
        createInfo.pNext = nullptr;
        createInfo.flags = 0;
        createInfo.image = *image;
        createInfo.viewType = type;
        createInfo.format = format;
        createInfo.subresourceRange = subresource;
        createInfo.components.r = VK_COMPONENT_SWIZZLE_R;
        createInfo.components.g = VK_COMPONENT_SWIZZLE_G;
        createInfo.components.b = VK_COMPONENT_SWIZZLE_B;
        createInfo.components.a = VK_COMPONENT_SWIZZLE_A;
        VK_CHECK(vkCreateImageView(*device, &createInfo, nullptr, &imageView));

        SharedPtr<GIImageViewVk> result(new GIImageViewVk(device));
        result->mCacheIndex = ComputeCacheIndex(type, format, subresource);
        result->mImage = image;
        result->mImageViewHandle = imageView;
        result->mImageViewType = type;
        result->mFormat = format;
        result->mSubresourceRange = subresource;

        return result;
    }

    CACHE_INDEX GIImageViewVk::ComputeCacheIndex(
        VkImageViewType type,
        VkFormat format,
        const VkImageSubresourceRange& subresource
    )
    {
        return std::hash<uint32_t>()(type)
            ^ std::hash<uint32_t>()(format) 
            ^ std::hash<uint32_t>()(subresource.aspectMask)
            ^ std::hash<uint32_t>()(subresource.baseArrayLayer)
            ^ std::hash<uint32_t>()(subresource.baseMipLevel)
            ^ std::hash<uint32_t>()(subresource.layerCount)
            ^ std::hash<uint32_t>()(subresource.levelCount);
    }

    GIImageViewVk::GIImageViewVk(SharedPtr<GIDeviceVk> device)
        : GIDeviceObjectVk(device)
    {}

    GIImageViewVk::~GIImageViewVk()
    {
        if (IsValid())
        {
            vkDestroyImageView(*mDevice, mImageViewHandle, nullptr);
            mImageViewHandle = VK_NULL_HANDLE;
        }
    }

    bool GIImageViewVk::IsValid() const
    {
        return mImageViewHandle != VK_NULL_HANDLE;
    }

    void GIImageViewVk::SetDebugName(const char* name) const
    {
        SetDebugNameInternal(
           mImageViewHandle,
           VK_DEBUG_REPORT_OBJECT_TYPE_IMAGE_VIEW_EXT,
           name
       );
    }

    void GIImageViewVk::SetDebugTag(const DebugTag& tag) const
    {
        SetDebugTagInternal(
           mImageViewHandle,
           VK_DEBUG_REPORT_OBJECT_TYPE_IMAGE_VIEW_EXT,
           tag
       );
    }

    GIImageViewVk::operator const VkImageView& () const
    {
        return mImageViewHandle;
    }

    SharedPtr<GIImageVk> GIImageViewVk::GetImage() const
    {
        return mImage;
    }

    VkImageViewType GIImageViewVk::GetImageViewType() const
    {
        return mImageViewType;
    }

    VkFormat GIImageViewVk::GetFormat() const
    {
        return mFormat;
    }

    VkImageSubresourceRange const& GIImageViewVk::GetSubresourceRange() const
    {
        return mSubresourceRange;
    }
}