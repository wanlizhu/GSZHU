#pragma once

#include "Graphics/RHI/Constants.h"
#include "Graphics/RHI/IDeviceResource.h"
#include "Graphics/RHI/IBufferView.h"

namespace ZHU
{
    struct ZHU_GS_API BufferDesc: public IDeviceResourceDesc
    {
        uint32_t SizeInBytes = 0;
		EBindFlags BindFlags = EBindFlags::None;
        EUsage Usage = EUsage::Default;
        EAccess CPUAccess = EAccess::None;
        EBufferMode Mode = EBufferMode::Undefined;

        // For a structured buffer, this defines the size of each buffer element.
        // For a formatted buffer, this member defines the size of the format that will be used for views created for this buffer.
        uint32_t ElementByteStride = 0;
        uint64_t CommandQueueMask = 1; // Defines which command queues this buffer can be used with
    };


    class ZHU_GS_API IBuffer : public IDeviceResource
    {
    public:
        using Pointer = std::shared_ptr<IBuffer>;
        using Desc = BufferDesc;

        virtual const BufferDesc& GetDesc() const = 0;
        // Buffer view will contain strong reference to the buffer.
        virtual IDeviceObject::Pointer CreateView(const BufferViewDesc& viewDesc) = 0;
        // Default views are only created for structured and raw buffers.
        // As, for formatted buffers, the view format is unknown at buffer initialization time
        virtual IDeviceObject::Pointer GetDefaultView(EBufferView viewType) const = 0;
        virtual void* GetNativeHandle() const = 0;
        virtual void SetState(EResourceState state) = 0;
        virtual EResourceState GetState() const = 0;
    };
}