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
            SharedPtr<GIImageVk> texture,
            VkImageViewType type,
            VkFormat format,
            const VkImageSubresourceRange& subresource = {}
        );

        static CACHE_INDEX ComputeCacheIndex(
            VkImageViewType type,
            VkFormat format,
            const VkImageSubresourceRange& subresource = {}
        );

        virtual ~GIImageViewVk();
        virtual bool IsValid() const override;
        virtual void SetDebugName(const char* name) const override;
        virtual void SetDebugTag(const DebugTag& tag) const override;

        operator const VkImageView& () const;
        SharedPtr<GIImageVk> GetTexture() const;
        VkImageViewType GetImageViewType() const;
        VkFormat GetFormat() const;
        VkImageSubresourceRange const& GetSubresourceRange() const;

    protected:
        GIImageViewVk(
            SharedPtr<GIImageVk> texture,
            VkImageViewType type,
            VkFormat format,
            const VkImageSubresourceRange& subresource = {}
        );

    private:
        VkImageView mImageViewHandle = VK_NULL_HANDLE;
        SharedPtr<GIImageVk> mTexture;
        VkImageViewType mImageViewType;
        VkFormat mFormat;
        VkImageSubresourceRange mSubresourceRange;
    };
}