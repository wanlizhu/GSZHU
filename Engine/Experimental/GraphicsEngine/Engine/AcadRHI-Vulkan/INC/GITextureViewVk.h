#pragma once

#include "GIDeviceObjectVk.h"

namespace AutoCAD::Graphics::Engine
{
    class GITextureVk;

    class GITextureViewVk : public GIDeviceObjectVk
    {
        DECL_DEVICE_OBJECT(GITextureViewVk)
    public:
        static SharedPtr<GITextureViewVk> Create(
            SharedPtr<GITextureVk> texture,
            VkImageViewType type,
            VkFormat format,
            const VkImageSubresourceRange& subresource = {}
        );

        static CACHE_INDEX ComputeCacheIndex(
            VkImageViewType type,
            VkFormat format,
            const VkImageSubresourceRange& subresource = {}
        );

        virtual ~GITextureViewVk();
        virtual bool IsValid() const override;
        virtual void SetDebugName(const char* name) const override;
        virtual void SetDebugTag(const DebugTag& tag) const override;

        operator const VkImageView& () const;
        SharedPtr<GITextureVk> GetTexture() const;
        VkImageViewType GetImageViewType() const;
        VkFormat GetFormat() const;
        VkImageSubresourceRange const& GetSubresourceRange() const;

    protected:
        GITextureViewVk(
            SharedPtr<GITextureVk> texture,
            VkImageViewType type,
            VkFormat format,
            const VkImageSubresourceRange& subresource = {}
        );

    private:
        VkImageView mImageViewHandle = VK_NULL_HANDLE;
        SharedPtr<GITextureVk> mTexture;
        VkImageViewType mImageViewType;
        VkFormat mFormat;
        VkImageSubresourceRange mSubresourceRange;
    };
}