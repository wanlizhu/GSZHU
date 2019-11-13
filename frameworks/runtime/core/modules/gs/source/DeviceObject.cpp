#include "GS/GIDevice/DeviceObject.h"

namespace GS::GI
{
    DeviceObject::operator bool() const
    {
        return mHandle != nullptr && mDevice;
    }

    const char* DeviceObject::GetName() const
    {
        return mName.c_str();
    }

    void* DeviceObject::GetHandle() const
    {
        return mHandle;
    }

    Device::Ptr DeviceObject::GetDevice() const
    {
        return mDevice;
    }

    void DeviceObject::SetName(const char* name)
    {
        mName = name;
    }

    DeviceObject::DeviceObject(const char* name, Device::Ptr device)
        : mName(name)
        , mHandle(nullptr)
        , mDevice(device)
    {}
}