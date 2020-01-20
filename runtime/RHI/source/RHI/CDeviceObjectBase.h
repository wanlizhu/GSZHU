#pragma once

#include "RHI/IDeviceObject.h"
#include "Utils/Logger.h"

namespace Wanlix
{
    template<typename _Interface_>
    class CDeviceObjectBase : public _Interface_
    {
        static_assert(std::is_base_of_v<IDeviceObject, _Interface_>);
    public:
        virtual void SetName(const String& name) override final { mObjectName = name; }
        virtual const String& GetName() const override final { return mObjectName; }
        virtual Handle GetHandle() const override { return nullptr; }
        virtual const Desc& GetDesc() const override final { return mDesc; }
        virtual Device* GetDevice() const override final { return mDevice.get(); }

    protected:
        CDeviceObjectBase(Device* device,
                          const Desc& desc,
                          const String& name)
            : mDevice(ToPtr<Device>(device->GetSharedPtr()))
            , mDesc(desc)
            , mObjectName(name)
        {}

    protected:
        SharedPtr<Device> mDevice;
        Desc mDesc;
        String mObjectName;
    };

}
