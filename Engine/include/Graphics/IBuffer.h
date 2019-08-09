#pragma once

#include "Graphics/IDeviceObject.h"
#include "Graphics/IBufferView.h"

namespace ZHU
{
    struct BufferDescriptor : public DeviceObjectDescriptor
    {
        uint32_t SizeInBytes = 0;
        BitSet<EBind> BindFlags = conv(EBind::None);
        EUsage Usage = EUsage::Default;
        BitSet<EMappedCPUAccess> MappedCPUAccess = conv(EMappedCPUAccess::None);
        EBufferMode Mode = EBufferMode::Undefined;

        // For a structured buffer, this defines the size of each buffer element.
        // For a formatted buffer, this member defines the size of the format that will be used for views created for this buffer.
        uint32_t ElementByteStride = 0;
        uint64_t CommandQueueMask = 1; // Defines which command queues this buffer can be used with
    };


    class ZHU_API IBuffer : public IDeviceObject
    {
    public:
        using Pointer = std::shared_ptr<IBuffer>;

        virtual const BufferDescriptor& GetDesc() const = 0;
        // Buffer view will contain strong reference to the buffer.
        virtual IDeviceObject::Pointer CreateView(const BufferViewDescriptor& viewDesc) = 0;
        // Default views are only created for structured and raw buffers.
        // As, for formatted buffers, the view format is unknown at buffer initialization time
        virtual IDeviceObject::Pointer GetDefaultView(EBufferView viewType) const = 0;
        virtual void* GetNativeHandle() const = 0;
        virtual void SetState(EResourceState state);
        virtual EResourceState GetState() const = 0;
    };
}