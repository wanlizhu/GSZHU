#pragma once

#include "GIResourceVk.h"

namespace AutoCAD::Graphics::Engine
{
    class GIBufferViewVk;
    class GICommandBufferVk;

    class GIBufferVk : public GIResourceVk
    {
        DECL_DEVICE_OBJECT(GIBufferVk)
    public:
        static SharedPtr<GIBufferVk> Create(
            SharedPtr<GIDeviceVk> device,
            VkDeviceSize size,
            VkBufferUsageFlagBits usage,
            std::vector<uint32_t> sharingQueues,
            VkMemoryPropertyFlags properties,
            const void* initialData,
            EResourceState initialState
        );

        virtual ~GIBufferVk();
        virtual bool IsValid() const override;
        virtual void SetDebugName(const char* name) const override;
        virtual void SetDebugTag(const DebugTag& tag) const override;
        virtual EResourceType GetResourceType() const override;
        virtual EResourceState GetResourceState() const override;
        virtual void TransitionState(const EResourceState& newState) override;
        virtual void SetOnDestroyCallback(const std::function<void()>& func) override;

        operator const VkBuffer& () const;
        VkDeviceMemory GetMemory() const;
        VkMemoryPropertyFlags GetMemoryProperties() const;
        VkDeviceSize GetSizeInBytes() const;
        VkBufferUsageFlags GetUsage() const;
        SharedPtr<GIBufferViewVk> GetBufferView(size_t offset, size_t size, VkFormat format);
        
        void* Map(size_t offset = 0, size_t size = VK_WHOLE_SIZE);
        void Unmap();
        void UpdateData(
            size_t offset,
            size_t size, 
            const void* data,
            SharedPtr<GICommandBufferVk> cmdbuf
        );
        
    private:
        GIBufferVk(SharedPtr<GIDeviceVk> device);

    private:
        VkBuffer mBufferHandle = VK_NULL_HANDLE;
        VkDeviceMemory mMemoryHandle = VK_NULL_HANDLE; // TODO: replace with device memory pool
        VkMemoryPropertyFlags mMemoryProperties = 0;
        VkDeviceSize mSizeInBytes = 0;
        VkBufferUsageFlags mUsage = VK_BUFFER_USAGE_FLAG_BITS_MAX_ENUM;
        
        std::unordered_map<CACHE_INDEX, WeakPtr<GIBufferViewVk>> mBufferViews;
        std::function<void()> mOnDestroyCallback;
        EResourceState mResourceState = EResourceState::Undefined;
        bool mIsMapped = false;
    };
}