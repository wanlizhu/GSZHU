#pragma once

#include "Graphics/RHI/IResource.h"

namespace Wanlix
{
    struct ResourceViewDesc : public DeviceObjectDesc
    {};

    class IResourceView : public IDeviceObject
    {
    public:
        virtual IResource* GetResource() const = 0;

        // Interfaces inherited from IDeviceObject
        virtual void SetName(const String& name) = 0;
        virtual const String& GetName() const = 0;
        virtual Handle ApiHandle() const = 0;
        virtual const ResourceViewDesc& GetDesc() const = 0;
    };
}