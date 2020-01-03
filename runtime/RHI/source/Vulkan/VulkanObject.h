#pragma once

#include "Wanlix/VulkanRHI/Common.h"

namespace Wanlix
{
    template<typename T>
    class VulkanObject : public NonCopyable
    {
    public:
        friend class LogicalDevice;
        using ObjectType = T;

        VulkanObject();
        VulkanObject(SharedPtr<LogicalDevice> device, T object);
        explicit VulkanObject(T object);
        VulkanObject(VulkanObject&& rhs);
        VulkanObject& operator=(VulkanObject&& rhs);
        ~VulkanObject();

        operator T() const;
        void Release();

    private:
        SharedPtr<LogicalDevice> mLogicalDevice;
        ObjectType mVkObject;
    };




    template<typename T>
    VulkanObject<T>::VulkanObject()
        : mLogicalDevice(nullptr)
        , mVkObject(VK_NULL_HANDLE)
    {}

    template<typename T>
    VulkanObject<T>::VulkanObject(SharedPtr<LogicalDevice> device, T object)
        : mLogicalDevice(device)
        , mVkObject(object)
    {}

    template<typename T>
    VulkanObject<T>::VulkanObject(T object)
        : mLogicalDevice(nullptr)
        , mVkObject(object)
    {}

    template<typename T>
    VulkanObject<T>::VulkanObject(VulkanObject&& rhs)
        : mLogicalDevice(std::move(rhs.mLogicalDevice))
        , mVkObject(rhs.mVkObject)
    {
        rhs.mVkObject = VK_NULL_HANDLE;
    }

    template<typename T>
    VulkanObject<T>& VulkanObject<T>::operator=(VulkanObject&& rhs)
    {
        Release();
        mLogicalDevice = std::move(rhs.mLogicalDevice);
        mVkObject = rhs.mVkObject;
        rhs.mVkObject = VK_NULL_HANDLE;
        return *this;
    }

    template<typename T>
    VulkanObject<T>::~VulkanObject()
    {
        Release();
    }

    template<typename T>
    VulkanObject<T>::operator T() const 
    {
        return mVkObject;
    }

    template<typename T>
    void VulkanObject<T>::Release()
    {
        if (mLogicalDevice && mVkObject != VK_NULL_HANDLE) {
            
        }
        mVkObject = VK_NULL_HANDLE;
        mLogicalDevice.reset();
    }
}