#pragma once

#include "IDeviceObject.h"

namespace Wanlix
{
    struct CommandBufferDesc : public DeviceObjectDesc
    {};

    class ICommandBuffer : public inherit_shared_from_this<IDeviceObject, ICommandBuffer>
    {
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