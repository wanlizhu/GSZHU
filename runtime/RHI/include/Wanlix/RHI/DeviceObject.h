#pragma once

#include "Common.h"

namespace Wanlix
{
    class Device;

    class DeviceObject : public NonCopyable
    {
    public:
        using Handle   = void*;
        using SharedPtr = std::shared_ptr<DeviceObject>;
        using UniquePtr = std::unique_ptr<DeviceObject>;
        using WeakPtr   = std::weak_ptr<DeviceObject>;

        static DeviceObject* Find(const std::string& name);

        virtual ~DeviceObject();
        void SetName(const std::string& name);
        const std::string& GetName() const;

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