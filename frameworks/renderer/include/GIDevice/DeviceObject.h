#pragma once

#include "Device.h"

namespace GS2::GI
{
    class DeviceObject : public std::enable_shared_from_this<DeviceObject>
    {
    public:
        using SharedPtr = std::shared_ptr<DeviceObject>;
        using WeakPtr   = std::weak_ptr<DeviceObject>;
        using UniquePtr = std::unique_ptr<DeviceObject>;

        DeviceObject() = delete;
        virtual ~DeviceObject() { destroy(); }

        virtual void init() = 0;
        virtual void destroy() = 0;
        virtual operator bool() const;

        const char* getName() const;
        void* getHandle() const;
        Device::SharedPtr getDevice() const;

        void setName(const char* name);

    protected:
        DeviceObject(const char* name, Device::SharedPtr device);

    protected:
        std::string _name;
        void* _handle = nullptr;
        Device::SharedPtr _device; // must release resource before device
    };
}