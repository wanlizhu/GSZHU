#include "Wanlix/Core3D/Graphics/GObject.h"
#include "Wanlix/Core3D/Utility/Exceptions.h"

namespace Wanlix
{
    std::unordered_map<String, GObject*> GObject::mObjects;

    GObject::GObject(
        StringCRef name,
        std::shared_ptr<Device> device
    )
        : mName(name)
        , mDevice(device)
    {
        if (mObjects.find(name) != mObjects.end()) {
            throw UniqueNameExists(FUNCTION_AND_LINE);
        }
        mObjects[name] = this;
    }

    GObject::~GObject()
    {
        auto it = mObjects.find(mName);
        if (it != mObjects.end()) {
            mObjects.erase(it);
        }
    }

    GObject* GObject::Find(StringCRef name)
    {
        auto it = mObjects.find(name);
        if (it == mObjects.end()) {
            return nullptr;
        }
        return it->second;
    }
    
    void GObject::SetName(StringCRef name)
    {
        mName = name;
    }

    StringCRef GObject::GetName() const
    {
        return mName;
    }
}