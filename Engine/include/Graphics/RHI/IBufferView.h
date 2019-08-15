#pragma once

#include "Graphics/IDeviceObject.h"
#include "Graphics/BufferFormat.h"
#include "Graphics/DeviceObjectDesc.h"

namespace ZHU
{
    class IBuffer;

    struct BufferViewDesc : public DeviceObjectDesc
    {
        EBufferView Type = EBufferView::Undefined;
        BufferFormat Format;
        uint32_t ByteOffset = 0;
        uint32_t SizeInBytes = 0;
    };


    class ZHU_API IBufferView : public IDeviceObject
    {
    public:
        using Pointer = std::shared_ptr<IBufferView>;
        using Desc = BufferViewDesc;

        virtual const BufferViewDesc& GetDesc() const = 0;
        virtual IBuffer& GetBuffer() const = 0;
    };
}