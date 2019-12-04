#include "Wanlix/RHI/DeviceObject.h"

namespace Wanlix
{
    std::unordered_map<std::string, DeviceObject*> DeviceObject::mObjects;

    DeviceObject::DeviceObject(
        const std::string&      name,
        std::shared_ptr<Device> device
    ) noexcept
        : mName(name)
        , mDevice(device)
    {
        if (mObjects.find(name) != mObjects.end()) {
            throw std::runtime_error("Device object name must be unique");
        }
        mObjects[name] = this;
    }

    DeviceObject::~DeviceObject()
    {
        auto it = mObjects.find(mName);
        if (it != mObjects.end()) {
            mObjects.erase(it);
        }
    }

    DeviceObject* DeviceObject::Find(const std::string& name)
    {
        auto it = mObjects.find(name);
        if (it == mObjects.end()) {
            return nullptr;
        }
        return it->second;
    }

    void DeviceObject::SetName(const std::string& name)
    {
        mName = name;
    }

    const std::string& DeviceObject::GetName() const
    {
        return mName;
    }
}