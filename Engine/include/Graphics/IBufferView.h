#pragma once

#include "Graphics/IDeviceObject.h"
#include "Graphics/BufferFormat.h"
#include "Graphics/DeviceObjectDescriptor.h"

namespace ZHU
{
    class IBuffer;

    struct BufferViewDescriptor : public DeviceObjectDescriptor
    {
        EBufferView Type = EBufferView::Undefined;
        BufferFormat Format;
        uint32_t ByteOffset = 0;
        uint32_t SizeInBytes = 0;
    };


    class ZHU_API IBufferView : public IDeviceObject
    {
    public:
        virtual const BufferViewDescriptor& GetDesc() const = 0;
        virtual IBuffer& GetBuffer() const = 0;
    };
}