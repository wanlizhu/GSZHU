#include "Wanlix/Core3D/RHI/IDeviceObject.h"

namespace Wanlix
{
    IDeviceObject::IDeviceObject(const std::string& name)
        : mName(name)
        , mHandle(nullptr)
    {}

    IDeviceObject::~IDeviceObject()
    {
        DestroyInternal();
    }

    void IDeviceObject::SetName(const std::string& name)
    {
        mName = name;
    }

    const std::string& IDeviceObject::GetName() const
    {
        return mName;
    }

    IDeviceObject::Handle IDeviceObject::GetHandle() const
    {
        return mHandle;
    }
}