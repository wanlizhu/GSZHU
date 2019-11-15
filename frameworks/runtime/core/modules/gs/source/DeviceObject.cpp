#include "GS/GIDevice/DeviceObject.h"

namespace GS::GI
{
    DeviceObject::operator bool() const
    {
        return mHandle != nullptr && mDevice;
    }

    const std::string& DeviceObject::GetName() const
    {
        return mName.c_str();
    }

    void* DeviceObject::GetHandle() const
    {
        return mHandle;
    }

    GIDevice::Ptr DeviceObject::GetDevice() const
    {
        return mDevice;
    }

    void DeviceObject::SetName(const std::string& name)
    {
        mName = name;
    }

    DeviceObject::DeviceObject(const std::string& name, GIDevice::Ptr device)
        : mName(name)
        , mHandle(nullptr)
        , mDevice(device)
    {}
}