#pragma once

#include "GIDevice.h"

namespace GS
{
    class GIDeviceObject : public std::enable_shared_from_this<GIDeviceObject>
                         , public GINotCopyable
                         , public GINotMovable
    {
    public:
        using Ptr = std::shared_ptr<GIDeviceObject>;
        using ConstPtr = std::shared_ptr<const GIDeviceObject>;
        using Handle = void*;

        static Ptr Create(const std::string& name, GIDevice::Ptr device);

        virtual ~GIDeviceObject() { Destroy(); }
        virtual void Init(void* data) = 0;
        virtual void Destroy() = 0;

        const std::string& GetName() const;
        void* GetHandle() const;
        GIDevice::Ptr GetDevice() const;

        void SetName(const std::string& name);

    protected:
        GIDeviceObject(const std::string& name, GIDevice::Ptr device);

    protected:
        std::string mName;
        Handle mHandle = nullptr;
        GIDevice::Ptr mDevice; // must release resource before device
    };
}