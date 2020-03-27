#pragma once

#include "GIDeviceObjectVk.h"

namespace AutoCAD::Graphics::Engine
{
    class GIBufferViewVk;

    class GIBufferVk : public GIDeviceObjectVk
    {
        DECL_DEVICE_OBJECT(GIBufferVk)
    public:
        struct ViewCacheKey
        {
            size_t offset = 0;
            size_t size = 0;
            VkFormat format = VK_FORMAT_UNDEFINED;

            ViewCacheKey() = default;
            ViewCacheKey(size_t _offset, size_t _size, VkFormat _format);
            size_t operator()(const ViewCacheKey& key) const;
            bool operator==(const ViewCacheKey& rhs) const;
            bool operator!=(const ViewCacheKey& rhs) const;
        };

        static SharedPtr<GIBufferVk> Create(
            SharedPtr<GIDeviceVk> device,
            VkDeviceSize size,
            VkBufferUsageFlags usages,
            VkMemoryPropertyFlags properties,
            const std::vector<uint32_t>& queues = {},
            const void* data = nullptr);

        virtual ~GIBufferVk();
        virtual bool IsValid() const override; // Will be overrided by index/vertex buffer
        virtual void SetDebugName(const char* name) const override;
        virtual void SetDebugTag(const DebugTag& tag) const override;

        operator const VkBuffer& () const;
        void WriteData(size_t offset, size_t size, const void* data);
        void* Map(size_t offset = 0, size_t size = VK_WHOLE_SIZE);
        void Unmap();
        VkBufferUsageFlags GetUsages() const;
        SharedPtr<GIBufferViewVk> GetBufferView(size_t offset, size_t size, VkFormat format);

    protected:
        GIBufferVk(
            SharedPtr<GIDeviceVk> device,
            VkDeviceSize size,
            VkBufferUsageFlags usages,
            VkMemoryPropertyFlags properties,
            const std::vector<uint32_t>& queues = {},
            const void* data = nullptr);

    protected:
        VkBuffer mBuffer = VK_NULL_HANDLE;
        VkDeviceMemory mDeviceMemory = VK_NULL_HANDLE; // TODO: replace with device memory pool
        VkDeviceSize mSize = 0;
        VkBufferUsageFlags mUsages = 0;
        bool mIsMapped = false;

        std::unordered_map<ViewCacheKey, WeakPtr<GIBufferViewVk>, ViewCacheKey> mBufferViews;
    };
}