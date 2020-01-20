#pragma once

#include "RHI/IDeviceObject.h"

namespace Wanlix
{
    struct CommandQueueDesc : public DeviceObjectDesc
    {};

    class ICommandQueue : public IDeviceObject
    {
    public:
        virtual Uint64 GetNextFenceValue() const = 0;
        virtual Uint64 GetCompletedFenceValue() const = 0;
        virtual void SignalFence(IFence* fence, Uint64 value) = 0;
        virtual Uint64 WaitForIdle() = 0;
        virtual Uint64 Submit(ICommandList* commandList) = 0;
        virtual ICommandList* GetCommandList() const = 0;
    };
}