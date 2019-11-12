#include "HWGI/DeviceObject.h"

namespace GS2
{
    DeviceObject::operator bool() const
    {
        return _handle != nullptr && _device;
    }

    const char* DeviceObject::getName() const
    {
        return _name.c_str();
    }

    void* DeviceObject::getHandle() const
    {
        return _handle;
    }

    Device::SharedPtr DeviceObject::getDevice() const
    {
        return _device;
    }

    void DeviceObject::setName(const char* name)
    {
        _name = name;
    }

    DeviceObject::DeviceObject(const char* name, Device::SharedPtr device)
        : _name(name)
        , _handle(nullptr)
        , _device(device)
    {}
}