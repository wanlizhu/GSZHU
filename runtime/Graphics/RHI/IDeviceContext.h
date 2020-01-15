#pragma once

#include "RHI/IBuffer.h"
#include "RHI/ITexture.h"
#include "RHI/IFence.h"

namespace Wanlix
{
    struct DeviceContextData
    {
        ECommandQueue queueType;
        Handle commandBuffer;
        Handle commandQueue;
        Handle commandAllocator;
        SharedPtr<IFence> fence;
    };

    class IDeviceContext
    {
    public:
        virtual void Flush(bool wait = false) = 0;
        virtual bool HasPendingCommands() const = 0;
        virtual bool SetPendingCommands(bool flag) const = 0;
        virtual bool AddResourceBarrier(const IResourceView& view, EResourceState newState) = 0;
        virtual Handle GetCommandBuffer() const = 0;
        virtual const DeviceContextData& GetDeviceContextData() const = 0;
        virtual void SetDeviceContextData(const DeviceContextData& data) = 0;
        virtual void BindDescriptorHeaps() = 0;
    };
}