#pragma once

#include "RHI/IDevice.h"
#include "Utils/NonCopyable.h"
#include <type_traits>

namespace Wanlix
{
    struct DeviceObjectDesc {};

    class IDeviceObject : public NonCopyable 
                        , public std::enable_shared_from_this<IDeviceObject>
    {
    public:
        virtual IDevice* GetDevice() const = 0;
        virtual const DeviceObjectDesc& GetDesc() const = 0;

        inline Bool SharedFromThisEnabled() const
        {
            return !weak_from_this().expired();
        }

        template<typename T>
        inline SharedPtr<T> GetSharedPtr() 
        {
            static_assert(std::is_base_of_v<IDeviceObject, T>);
            assert(SharedFromThisEnabled());
            return std::dynamic_pointer_cast<T>(shared_from_this());
        };
    };
}