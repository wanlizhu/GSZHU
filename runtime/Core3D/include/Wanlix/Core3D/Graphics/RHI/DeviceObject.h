#pragma once

#include "Wanlix/Core3D/Utility/NonCopyable.h"
#include <cstdint>
#include <string>
#include <memory>
#include <unordered_map>

namespace Wanlix
{
    class DeviceObject : public NonCopyable
    {
    public:
        using Handle = void*;
        using Ptr = std::shared_ptr<DeviceObject>;
        using ConstPtr = std::shared_ptr<const DeviceObject>;
        using WeakPtr = std::weak_ptr<DeviceObject>;

        static DeviceObject* Find(const std::string& name);
        virtual ~DeviceObject();
        
        void SetName(const std::string& name);
        const std::string& GetName() const;
        Handle GetHandle() const;

    protected:
        DeviceObject(const std::string& name);

    protected:
        std::string mName;
        Handle mHandle = nullptr;

    private:
        static std::unordered_map<std::string, DeviceObject*> mObjects;
    };
}