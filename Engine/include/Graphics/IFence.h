#pragma once

#include "Graphics/IDeviceObject.h"

namespace ZHU
{
    struct FenceDescriptor : public DeviceObjectDescriptor
    {};


    class ZHU_API IFence : public IDeviceObject
    {
    public:
        using Pointer = std::shared_ptr<IFence>;

        virtual const FenceDescriptor& GetDesc() const = 0;
        virtual uint64_t GetCompletedValue() const = 0;
        virtual void Reset(uint64_t value) = 0;
    };
}