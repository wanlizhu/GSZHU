#pragma once

#include "IDeviceObject.h"

namespace Wanlix
{
    struct CommandBufferDesc : public DeviceObjectDesc
    {};

    class RHI_API ICommandBuffer : public IDeviceObject
    {
        CLASSINFO(ICommandBuffer)
        INHERIT_SHARED_FROM_THIS(IDeviceObject)
    public:
        virtual ~ICommandBuffer() = default;

        /* interfaces inherited from IDeviceObject */
        virtual Pvoid GetNativeHandle() const override = 0;
        virtual CommandBufferDesc const& GetDesc() const override = 0;
        virtual IDevice* GetDevice() const override = 0;

        virtual void Begin() = 0;
        virtual void End() = 0;
        virtual void Execute() = 0;

        virtual void PushDebugGroup(String const& name) = 0;
        virtual void PopDebugGroup() = 0;
    };
}