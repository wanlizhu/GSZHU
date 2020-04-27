#pragma once

#include "GIDeviceObjectVk.h"

namespace AutoCAD::Graphics::Engine
{
    class GIBufferVk;

    class GIBufferViewVk : public GIDeviceObjectVk
    {
        DECL_DEVICE_OBJECT(GIBufferViewVk)
    public:
        static SharedPtr<GIBufferViewVk> Create(
            SharedPtr<GIBufferVk> buffer,
            size_t offset,
            size_t size,
            VkFormat format
        );

        static CACHE_INDEX ComputeCacheIndex(
            size_t offset, 
            size_t size,
            VkFormat format
        );

        virtual ~GIBufferViewVk();
        virtual bool IsValid() const override;
        virtual void SetDebugName(const char* name) const override;
        virtual void SetDebugTag(const DebugTag& tag) const override;

        operator const VkBufferView& () const;
        SharedPtr<GIBufferVk> GetBuffer() const;
        size_t GetOffset() const;
        size_t GetSize() const;
        VkFormat GetFormat() const;

    protected:
        GIBufferViewVk(
            SharedPtr<GIBufferVk> buffer,
            size_t offset,
            size_t size,
            VkFormat format
        );

    private:
        CACHE_INDEX mCacheIndex = 0;
        VkBufferView mBufferViewHandle = VK_NULL_HANDLE;
        SharedPtr<GIBufferVk> mBuffer;
        size_t mOffset = 0;
        size_t mSize = 0;
        VkFormat mFormat = VK_FORMAT_UNDEFINED;
    };
}