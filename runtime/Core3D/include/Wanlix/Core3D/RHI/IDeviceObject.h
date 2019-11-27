#pragma once

#include "Wanlix/Core3D/Utility/NonCopyable.h"
#include <cstdint>
#include <string>
#include <memory>

namespace Wanlix
{
    class IDeviceObject : public NonCopyable
    {
    public:
        using Handle = void*;
        using Ptr = std::shared_ptr<IDeviceObject>;
        using ConstPtr = std::shared_ptr<const IDeviceObject>;

        virtual ~IDeviceObject();
        
        void SetName(const std::string& name);
        const std::string& GetName() const;
        Handle GetHandle() const;

    protected:
        IDeviceObject(const std::string& name);
        virtual bool CreateInternal() = 0;
        virtual void DestroyInternal() = 0;

    protected:
        std::string mName;
        Handle mHandle = nullptr;
    };
}