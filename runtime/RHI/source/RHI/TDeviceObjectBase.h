#pragma once

#include "RHI/IDeviceObject.h"
#include "Utils/Logger.h"

namespace Wanlix
{
    template<typename _Interface_>
    class TDeviceObjectBase : public _Interface_
    {
        static_assert(std::is_base_of_v<IDeviceObject, _Interface_> || std::is_same_v<IDeviceObject, _Interface_>);
        using _Device_ = typename _Interface_::Device;
        using _Desc_ = typename _Interface_::Desc;

    public:
        virtual void SetName(const String& name) override final { mObjectName = name; }
        virtual const String& GetName() const override final { return mObjectName; }
        virtual Handle GetHandle() const override { return nullptr; }
        virtual const DescType& GetDesc() const override final { return mDesc; }
        virtual DeviceType* GetDevice() const override final { return mDevice.get(); }

    protected:
        TDeviceObjectBase(DeviceType* device,
                          const DescType& desc,
                          const String& name)
            : mDevice(device->GetSharedPtr())
            , mDesc(desc)
            , mObjectName(name)
        {}

    protected:
        SharedPtr<_Device_> mDevice;
        DescType mDesc;
        String mObjectName;
    };

}
