#include "Graphics/IDeviceObject.h"

namespace ZHU
{
    IDeviceObject::IDeviceObject(const std::string& name)
        : Object(name)
    {}

    IDeviceObject::Pointer IDeviceObject::FindDeviceObject(const std::string& name)
    {
        auto it = smDeviceObjects.find(name);
        if (it != smDeviceObjects.end()) {
            auto shared = it->second.lock();
            if (shared) {
                return shared;
            }
            smDeviceObjects.erase(it);
        }

        return std::shared_ptr<IDeviceObject>(nullptr);
    }

    size_t IDeviceObject::GetDeviceObjectCount()
    {
        return smDeviceObjects.size();
    }

    std::weak_ptr<IDeviceObject> IDeviceObject::GetDeviceObject(size_t index)
    {
        auto it = smDeviceObjects.begin();
        while (index--) {
            it++;
        }

        return it == smDeviceObjects.end() ? std::weak_ptr<IDeviceObject>() : it->second;
    }
}