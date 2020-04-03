#pragma once

#include "GIResourceVk.h"

namespace AutoCAD::Graphics::Engine
{
    class GIBufferViewVk;
    class GICommandBufferVk;

    struct GIBufferInfoVk 
    {
        VkDeviceSize sizeInBytes = 0;
        VkBufferUsageFlags usages = 0;
        VkSharingMode sharingMode = VK_SHARING_MODE_EXCLUSIVE;
    };

    class GIBufferVk : public GIResourceVk
    {
        friend class GIBufferBuilderVk;
        friend class GIBufferBuilderVMA;
        DECL_DEVICE_OBJECT(GIBufferVk)
    public:
        virtual ~GIBufferVk();
        virtual bool IsValid() const override final;
        virtual void SetDebugName(const char* name) const override final;
        virtual void SetDebugTag(const DebugTag& tag) const override final;
        virtual EResourceType GetResourceType() const override final;
        virtual GIResourceStateVk& GetResourceState() override final;
        virtual void TransitionState(const GIResourceStateVk& newState) override final;

        operator const VkBuffer& () const;
        GIBufferInfoVk const& GetInfo() const;
        SharedPtr<GIBufferViewVk> GetBufferView(size_t offset, size_t size, VkFormat format);
        
        void SetOnDestroyCallback(const std::function<void()>& func);
        void* Map(size_t offset = 0, size_t size = VK_WHOLE_SIZE);
        void Unmap();
        void UpdateData(size_t offset, size_t size, const void* data, SharedPtr<GICommandBufferVk> cmdbuf = nullptr);
        
    private:
        GIBufferVk(
            SharedPtr<GIDeviceVk> device,
            const VkBufferCreateInfo& createInfo,
            const void* data,
            VkMemoryPropertyFlags properties,
            EResourceState initialState
        ); /* [1] Create buffer, create device memory required and bind them together */

        GIBufferVk(
            SharedPtr<GIDeviceVk> device,
            const GIBufferInfoVk& info,
            EResourceState initialState
        ); /* [2] Configure with buffer and memory objects precreated by VMA */

    private:
        VkBuffer mBufferHandle = VK_NULL_HANDLE;
        VkDeviceMemory mMemoryHandle = VK_NULL_HANDLE; // TODO: replace with device memory pool
        GIBufferInfoVk mBufferInfo;
        
        std::unordered_map<CACHE_INDEX, WeakPtr<GIBufferViewVk>> mBufferViews;
        std::function<void()> mOnDestroyCallback;
        GIResourceStateVk mResourceState;
        bool mIsMapped = false;
    };
}