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
            assert(false);
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

    bool DeviceObject::IsRefCounted() const
    {
        return !weak_from_this().expired();
    }
}