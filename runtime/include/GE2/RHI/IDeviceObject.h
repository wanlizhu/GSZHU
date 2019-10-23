#pragma once

#include "GE2/Utilities.h"

namespace GE2::RHI
{
    class GE2_IMPEXP IDeviceObject : public std::enable_shared_from_this<IDeviceObject>
    {
    public:
        struct InitializeData {};
        IDeviceObject() = default;
        IDeviceObject(const IDeviceObject&) = delete;
        IDeviceObject(IDeviceObject&&) = delete;
        IDeviceObject& operator=(const IDeviceObject&) = delete;
        IDeviceObject& operator=(IDeviceObject&&) = delete;
        
        virtual ~IDeviceObject() {};
        virtual bool   Initialize(const InitializeData&) = 0;  // derived class could provide types inherit from InitializeData
        virtual void   Destroy() = 0;
        virtual CSTR   GetName() const = 0;
        virtual HANDLE GetHandle() const = 0;
    };
}