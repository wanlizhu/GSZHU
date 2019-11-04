#include "HWGI/Resource.h"

namespace GS2
{
    void Resource::init() 
    {}

    void Resource::destroy() 
    {}

    Resource::operator bool() const 
    {
        return DeviceObject::operator bool() && _type != EType::Undefined;
    }
        
    Resource::Resource(const char* name, Device::SharedPtr device, EType type)
        : DeviceObject(name, device)
        , _type(type)
    {}
}