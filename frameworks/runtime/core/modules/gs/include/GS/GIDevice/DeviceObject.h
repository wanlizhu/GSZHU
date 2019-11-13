#pragma once

#include "Device.h"

namespace GS::GI
{
    class DeviceObject : public std::enable_shared_from_this<DeviceObject>
    {
    public:
        using Ptr = std::shared_ptr<DeviceObject>;

        DeviceObject() = delete;
        virtual ~DeviceObject() { Destroy(); }

        virtual void Init() = 0;
        virtual void Destroy() = 0;
        virtual operator bool() const;

        const char* GetName() const;
        void* GetHandle() const;
        Device::Ptr GetDevice() const;

        void SetName(const char* name);

    protected:
        DeviceObject(const char* name, Device::Ptr device);

    protected:
        std::string mName;
        void* mHandle = nullptr;
        Device::Ptr mDevice; // must release resource before device
    };
}