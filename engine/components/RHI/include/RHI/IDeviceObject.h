#pragma once

#include "RHI/IDevice.h"
#include <type_traits>

namespace Wanlix
{
    struct DeviceObjectDesc {};

    class IDeviceObject : public std::enable_shared_from_this<IDeviceObject>
    {
    public:
        using Desc = DeviceObjectDesc;
        using Device = IDevice;

        virtual void SetName(const String& name) = 0;
        virtual const String& GetName() const = 0;
        virtual Handle GetHandle() const = 0;
        virtual const DeviceObjectDesc& GetDesc() const = 0;
        virtual IDevice* GetDevice() const = 0;

        template<typename T>
        inline SharedPtr<T> GetSharedPtr() 
        {
            static_assert(std::is_base_of_v<IDeviceObject, T>);
            assert(!weak_from_this().expired());
            return std::dynamic_pointer_cast<T>(shared_from_this());
        };
    };
}