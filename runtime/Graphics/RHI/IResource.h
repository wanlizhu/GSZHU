#pragma once

#include "Graphics/RHI/IDeviceObject.h"

namespace Wanlix
{
    struct ResourceDesc : public DeviceObjectDesc
    {};

    class IResource : public IDeviceObject
    {
    public:
        virtual EResourceType GetType() const = 0;
        
        // Interfaces inherited from IDeviceObject
        virtual void SetName(const String& name) = 0;
        virtual const String& GetName() const = 0;
        virtual Handle ApiHandle() const = 0;
        virtual const ResourceDesc& GetDesc() const = 0;
    };
}