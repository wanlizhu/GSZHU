#pragma once

#include "GIDeviceObjectVk.h"
#include "GIDescriptorResourceVk.h"

namespace AutoCAD::Graphics::Engine
{
    class GIBufferViewVk;

    class GIBufferVk 
        : public GIDeviceObjectVk
        , public GIDescriptorResourceVk
    {
        DECL_DEVICE_OBJECT(GIBufferVk)
    public:
        static SharedPtr<GIBufferVk> Create(
            VkDeviceSize size,
            VkBufferUsageFlags usage,
            VkMemoryPropertyFlags properties,
            const void* data = nullptr);

        virtual ~GIBufferVk();
        virtual bool IsValid() const override;
        virtual void SetDebugName(const char* name) const override;
        virtual void SetDebugTag(const DebugTag& tag) const override;

        virtual size_t GetResourceSize() const override final;
        virtual uint32_t GetUnderlyingCopyCount() const override final;
        virtual VkDescriptorType GetResourceType() const override final;
        virtual VkAccessFlags GetResourceState() const override final;

        operator const VkBuffer& () const;
        void WriteData(size_t offset, size_t size, const void* data);
        void* Map(size_t offset = 0, size_t size = VK_WHOLE_SIZE);
        void Unmap();
        VkBufferUsageFlags GetUsages() const;
        VkAccessFlags GetResourceRangeState(size_t offset, size_t size) const;
        SharedPtr<GIBufferViewVk> GetBufferView(size_t offset, size_t size, VkFormat format);

    protected:
        void SetResourceState(VkAccessFlags state);
        void SetResourceRangeState(size_t offset, size_t size, VkAccessFlags state);

    protected:
        VkBuffer mBuffer = VK_NULL_HANDLE;
        VkDeviceMemory mDeviceMemory = VK_NULL_HANDLE; // TODO: replace with device memory pool
        VkDeviceSize mSize = 0;
        VkBufferUsageFlags mUsage = 0;
        
        std::unordered_map<std::pair<size_t, size_t>, VkAccessFlags> mResourceRangeStates;
        std::unordered_map<std::pair<size_t, size_t>, void*> mMappedDataRanges;
        std::unordered_map<std::pair<size_t, size_t>, WeakPtr<GIBufferViewVk>> mBufferViews;
    };
}