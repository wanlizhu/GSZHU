#pragma once

#include <memory>
#include "ObjectBase.h"

namespace NTGS {
template<typename BaseInterface, typename RenderDeviceType, typename ObjectDescType>
class DeviceObjectBase : public ObjectBase<BaseInterface> {
public:
    using BaseType = ObjectBase<BaseInterface>;

    DeviceObjectBase(Pointer<RenderDeviceType> Device, const ObjectDescType& Desc)
        : m_Device(Device)
        , m_Desc(Desc) 
    {}
    DeviceObjectBase(const DeviceObjectBase&) = delete;
    DeviceObjectBase(DeviceObjectBase&&) = delete;
    DeviceObjectBase& operator = (const DeviceObjectBase&) = delete;
    DeviceObjectBase& operator = (DeviceObjectBase&&) = delete;
    virtual ~DeviceObjectBase() = default;

    inline virtual const DescType& GetDesc() const override final { return m_Desc; }
    inline Pointer<RenderDeviceType> GetDevice() const { return m_Device; }

protected:
    Pointer<RenderDeviceType> m_Device;
    ObjectDescType m_Desc;
};
}