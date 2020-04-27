#pragma once

#include <memory>
#include <functional>
#include <vulkan/vulkan.h>

namespace djv
{
    template<typename T>
    class VulkanHandle
    {
    public:
        using Handle = T;
        using Deleter = std::function<void()>;
        struct Hash 
        {
            size_t operator()(const VulkanHandle<T>& handle) const
            {
                return std::hash<void*>()(handle.handle());
            }
        };

        VulkanHandle() = default;
        VulkanHandle(nullptr_t) {}
        VulkanHandle(Handle handle_, const Deleter& deleter)
            : mHandle(handle_)
            , mWatcher(std::shared_ptr<VulkanHandle<T>>(
                this, 
                [=] (VulkanHandle<T>* self) {
                    deleter();
                    self->mHandle = VK_NULL_HANDLE;
                })
            )
        {}

        inline operator T() const { return mHandle; }
        inline VulkanHandle& operator=(nullptr_t) { reset(); return *this; }
        inline bool operator==(const VulkanHandle& rhs) const { return handle() == rhs.handle(); }
        inline bool operator!=(const VulkanHandle& rhs) const { return handle() != rhs.handle();}
        inline bool operator< (const VulkanHandle& rhs) const { return handle() <  rhs.handle();}

        inline T handle() const { return mHandle; }
        inline bool isValid() const { return mHandle != VK_NULL_HANDLE; }
        inline void reset() { mWatcher.reset(); }

    private:
        Handle mHandle = VK_NULL_HANDLE;
        std::shared_ptr<VulkanHandle<T>> mWatcher;
    };
}