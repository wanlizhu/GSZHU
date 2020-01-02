#pragma once

#include "IDeviceObject.h"

namespace Wanlix
{
    class RHI_API IResource : public IDeviceObject
    {
        CLASSINFO(IResource)
        INHERIT_SHARED_FROM_THIS(IDeviceObject)
    public:
        virtual ~IResource() = default;

        /* interfaces inherited from IDeviceObject */
        virtual void* GetNativeHandle() const override = 0;
        virtual DeviceObjectDesc const& GetDesc() const override = 0;
        virtual IDevice* GetDevice() const override = 0;

        virtual Size GetSizeInBytes() const = 0;
        virtual Uint GetBindFlags() const = 0;
        virtual ResourceType  GetType() const = 0;
        virtual ResourceState GetState() const = 0;
        virtual void SetState(ResourceState state) = 0;
    };
}