#pragma once

#include "Common.h"

namespace Wanlix
{
    class Device;

    struct DeviceObjectDescriptor {};

    class DeviceObject : public NonCopyable
                       , public std::enable_shared_from_this<DeviceObject>
    {
    public:
        using Handle   = void*;
        using SharedPtr = std::shared_ptr<DeviceObject>;
        using UniquePtr = std::unique_ptr<DeviceObject>;
        using WeakPtr   = std::weak_ptr<DeviceObject>;
        using Desc      = DeviceObjectDescriptor;

        static DeviceObject* Find(const std::string& name);

        virtual ~DeviceObject();
        void SetName(const std::string& name);
        
        const std::string& GetName() const;
        bool IsRefCounted() const;

    protected:
        DeviceObject(
            const std::string&      name,
            std::shared_ptr<Device> device
        ) noexcept;

    protected:
        std::string mName;
        std::shared_ptr<Device> mDevice;

    private:
        static std::unordered_map<std::string, DeviceObject*> mObjects;
    };
}