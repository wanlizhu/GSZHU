#pragma once

#include "Graphics/RHI/IDeviceObject.h"

namespace ZHU
{
    class IBuffer;

    struct ZHU_GS_API BufferViewDesc : public IDeviceObjectDesc
    {
		struct ZHU_GS_API BufferFormat
		{
			EComponent Component = EComponent::Undefined;
			uint8_t NumComponents = 0;
			bool IsNormalized = false;
		};

        EBufferView Type = EBufferView::Undefined;
        BufferFormat Format;
        uint32_t ByteOffset = 0;
        uint32_t SizeInBytes = 0;
    };


    class ZHU_GS_API IBufferView : public IDeviceObject
    {
    public:
        using Pointer = std::shared_ptr<IBufferView>;
        using Desc = BufferViewDesc;

        virtual const BufferViewDesc& GetDesc() const = 0;
        virtual IBuffer& GetBuffer() const = 0;
    };
}