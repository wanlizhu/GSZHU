#include "GS/GIDevice/Resource.h"

namespace GS::GI
{
    void Resource::Init() 
    {}

    void Resource::Destroy() 
    {}

    Resource::operator bool() const 
    {
        return DeviceObject::operator bool() && mType != EType::Undefined;
    }
        
    Resource::Resource(const char* name, Device::Ptr device, EType type)
        : DeviceObject(name, device)
        , mType(type)
    {}
}