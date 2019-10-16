#pragma once

#include "GE2/Utilities.h"

namespace GE2::RHI
{
    class GE2_IMPEXP IDeviceObject : public std::enable_shared_from_this<IDeviceObject>
    {
    public:
        using SharedPtr = std::shared_ptr<IDeviceObject>;
        using WeakPtr   = std::weak_ptr<IDeviceObject>;

        IDeviceObject() = default;
        IDeviceObject(const IDeviceObject&) = delete;
        IDeviceObject(IDeviceObject&&) = delete;
        IDeviceObject& operator=(const IDeviceObject&) = delete;
        IDeviceObject& operator=(IDeviceObject&&) = delete;
        
        virtual ~IDeviceObject() = 0;
        virtual bool   Initialize() = 0;
        virtual void   Destroy() = 0;
        virtual CSTR   GetName() const = 0;
        virtual HANDLE GetHandle() const = 0;
    };
}