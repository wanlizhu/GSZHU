#include "GIImageViewVk.h"
#include "GIImageVk.h"
#include "GIDeviceVk.h"

namespace AutoCAD::Graphics::Engine
{
    SharedPtr<GIImageViewVk> GIImageViewVk::Create(
        SharedPtr<GIImageVk> texture,
        VkImageViewType type,
        VkFormat format,
        const VkImageSubresourceRange& subresource
    )
    {
        return SharedPtr<GIImageViewVk>(new GIImageViewVk(
            texture,
            type,
            format,
            subresource
        ));
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

    GIImageViewVk::GIImageViewVk(
        SharedPtr<GIImageVk> texture,
        VkImageViewType type,
        VkFormat format,
        const VkImageSubresourceRange& subresource
    )
        : GIDeviceObjectVk(texture->GetDevice())
        , mTexture(texture)
        , mImageViewType(type)
        , mFormat(format)
        , mSubresourceRange(subresource)
    {
        VkImageViewCreateInfo createInfo = {};
        createInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
        createInfo.pNext = nullptr;
        createInfo.flags = 0;
        createInfo.image = *mTexture;
        createInfo.viewType = mImageViewType;
        createInfo.format = mFormat;
        createInfo.subresourceRange = mSubresourceRange;
        createInfo.components.r = VK_COMPONENT_SWIZZLE_R;
        createInfo.components.g = VK_COMPONENT_SWIZZLE_G;
        createInfo.components.b = VK_COMPONENT_SWIZZLE_B;
        createInfo.components.a = VK_COMPONENT_SWIZZLE_A;
        VK_CHECK(vkCreateImageView(*mDevice, &createInfo, nullptr, &mImageViewHandle));
    }

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

    SharedPtr<GIImageVk> GIImageViewVk::GetTexture() const
    {
        return mTexture;
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