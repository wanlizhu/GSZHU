#pragma once

#include "GIDeviceObjectVk.h"

namespace AutoCAD::Graphics::Engine
{
    class GIImageVk;

    class GIImageViewVk : public GIDeviceObjectVk
    {
        DECL_DEVICE_OBJECT(GIImageViewVk)
    public:
        static SharedPtr<GIImageViewVk> Create(
            SharedPtr<GIImageVk> image,
            VkImageViewType type,
            VkFormat format,
            const VkImageSubresourceRange& subresource
        );

        static CACHE_INDEX ComputeCacheIndex(
            VkImageViewType type,
            VkFormat format,
            const VkImageSubresourceRange& subresource
        );

        virtual ~GIImageViewVk();
        virtual bool IsValid() const override;
        virtual void SetDebugName(const char* name) const override;
        virtual void SetDebugTag(const DebugTag& tag) const override;

        operator const VkImageView& () const;
        SharedPtr<GIImageVk> GetImage() const;
        VkImageViewType GetImageViewType() const;
        VkFormat GetFormat() const;
        VkImageSubresourceRange const& GetSubresourceRange() const;

    protected:
        GIImageViewVk(SharedPtr<GIDeviceVk> device);

    private:
        CACHE_INDEX mCacheIndex = 0;
        VkImageView mImageViewHandle = VK_NULL_HANDLE;
        SharedPtr<GIImageVk> mImage;
        VkImageViewType mImageViewType;
        VkFormat mFormat;
        VkImageSubresourceRange mSubresourceRange;
    };
}