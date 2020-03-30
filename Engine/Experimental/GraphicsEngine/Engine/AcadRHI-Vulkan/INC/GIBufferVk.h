#pragma once

#include "GIDeviceResourceVk.h"

namespace AutoCAD::Graphics::Engine
{
    class GIBufferViewVk;
    class GICommandBufferVk;
    
    class GIBufferVk : public GIDeviceResourceVk
    {
        using ViewCacheHash = size_t;
        friend class GIBufferBuilderVk;
        DECL_DEVICE_OBJECT(GIBufferVk)
    public:
        virtual ~GIBufferVk();
        virtual bool IsValid() const override; // Will be overrided by index/vertex buffer
        virtual void SetDebugName(const char* name) const override;
        virtual void SetDebugTag(const DebugTag& tag) const override;
        virtual EResourceType GetResourceType() const override;

        operator const VkBuffer& () const;
        VkDeviceSize GetSize() const;
        VkDeviceMemory GetDeviceMemory() const;
        VkBufferUsageFlags GetUsages() const;
        SharedPtr<GIBufferViewVk> GetBufferView(size_t offset, size_t size, VkFormat format);
        void* Map(size_t offset = 0, size_t size = VK_WHOLE_SIZE);
        void Unmap();
        void UpdateData(size_t offset, size_t size, const void* data, SharedPtr<GICommandBufferVk> cmdbuf = nullptr);
        
    protected:
        GIBufferVk(
            SharedPtr<GIDeviceVk> device,
            const VkBufferCreateInfo& createInfo,
            const VkMemoryAllocateInfo& allocInfo,
            const void* data,
            bool useStagingBuffer
        );

    protected:
        VkBuffer mBuffer = VK_NULL_HANDLE;
        VkDeviceMemory mDeviceMemory = VK_NULL_HANDLE; // TODO: replace with device memory pool
        VkDeviceSize mDeviceMemorySize = 0;
        VkBufferUsageFlags mUsages = 0;
        bool mIsMapped = false;
        std::unordered_map<ViewCacheHash, WeakPtr<GIBufferViewVk>> mBufferViews;

    private:
        std::optional<VkBuffer> mStagingBuffer;
        std::optional<VkDeviceMemory> mStagingBufferMemory;
    };

    class GIBufferBuilderVk
    {
    public:
        GIBufferBuilderVk(SharedPtr<GIDeviceVk> device);
        GIBufferBuilderVk& SetSize(VkDeviceSize size);
        GIBufferBuilderVk& SetBufferUsage(VkBufferUsageFlags usages);
        GIBufferBuilderVk& SetMemoryProperties(VkMemoryPropertyFlags properties);
        GIBufferBuilderVk& AddSharedQueue(uint32_t queue);
        GIBufferBuilderVk& SetInitialData(const void* data);
        GIBufferBuilderVk& UseVMA(bool value);
        GIBufferBuilderVk& UseStagingBuffer(bool value);
        SharedPtr<GIBufferVk> Build();

    private:
        SharedPtr<GIDeviceVk> mDevice;
        VkBufferCreateInfo mCreateInfo = {};
        VkMemoryAllocateInfo mAllocInfo = {};

        const void* mInitialData = nullptr;
        bool mUseVMA = false;
        bool mUseStagingBuffer = false;
        std::vector<uint32_t> mSharedQueues;
    };
}