#pragma once

#include <memory>
#include <functional>
#include <type_traits>
#include <vulkan/vulkan.h>

namespace djv
{
    void delete_vulkan_handle(
        VkDevice device, 
        void* handle, 
        const VkAllocationCallbacks* allocator, 
        const std::type_info& type
    );

    template<typename T>
    class VulkanHandle
    {
    public:
        VulkanHandle() = default;
        VulkanHandle(nullptr_t) {}
        VulkanHandle(T handle_, const std::function<void()>& deleter)
            : mHandle(handle_)
            , mWatcher(this, [=] (VulkanHandle<T>* self) { if (self) deleter(); })
        {}
        VulkanHandle(T handle_, VkDevice device)
            : mHandle(handle_)
            , mWatcher(this, [=] (VulkanHandle<T>* self) {
                if (self && device) {
                    delete_vulkan_handle(device, self->mHandle, nullptr, typeid(T));
                    self->mHandle = nullptr;
                }
            })
        {}
        
        inline operator T() const { return mHandle; }
        inline T operator*() const { return mHandle; }
        inline VulkanHandle& operator=(nullptr_t) { reset(); return *this; }
        inline bool operator==(const VulkanHandle& rhs) const { return handle() == rhs.handle(); }
        inline bool operator!=(const VulkanHandle& rhs) const { return handle() != rhs.handle();}
        inline bool operator< (const VulkanHandle& rhs) const { return handle() <  rhs.handle();}

        inline T handle() const { return mHandle; }
        inline bool isValid() const { return mHandle != VK_NULL_HANDLE; }
        inline void reset() { mWatcher.reset(); mHandle = VK_NULL_HANDLE; }
        inline int  useCount() const { mWatcher.use_count(); }
        inline void swap(VulkanHandle<T>& other) { std::wap(mHandle, other.mHandle); std::swap(mWatcher, other.mWatcher); }

    private:
        T mHandle = VK_NULL_HANDLE;
        std::shared_ptr<VulkanHandle<T>> mWatcher;
    };
}