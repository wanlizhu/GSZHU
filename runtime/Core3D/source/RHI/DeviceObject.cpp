#include "Wanlix/Core3D/RHI/DeviceObject.h"
#include "Wanlix/Core3D/Utility/Exceptions.h"

namespace Wanlix
{
    std::unordered_map<std::string, DeviceObject*> DeviceObject::mObjects;

    DeviceObject::DeviceObject(const std::string& name)
        : mName(name)
        , mHandle(nullptr)
    {
        if (mObjects.find(name) != mObjects.end()) {
            throw UniqueNameExists(FUNCTION_AND_LINE);
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

    DeviceObject::Handle DeviceObject::GetHandle() const
    {
        return mHandle;
    }
}