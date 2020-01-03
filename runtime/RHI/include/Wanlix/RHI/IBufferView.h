#pragma once

#include "IBuffer.h"

namespace Wanlix
{
    struct BufferFormat
    {
        DataType dataType = DataType::Undefined;
        Uint components = 0;
        Bool isNormalized = false;
    };

    struct BufferViewDesc : public DeviceObjectDesc
    {
        BufferViewType viewType = BufferViewType::Undefined;
        BufferFormat format;
        Range range;
    };

    BufferViewDesc MakeBufferViewDesc(BufferViewType type,
                                      BufferFormat format,
                                      Range range);

    class RHI_API IBufferView : public IDeviceObject
    {
        CLASSINFO(IBufferView)
        INHERIT_SHARED_FROM_THIS(IDeviceObject)
    public:
        /* interfaces inherited from IDeviceObject */
        virtual Pvoid GetNativeHandle() const override = 0;
        virtual BufferViewDesc const& GetDesc() const override = 0;
        virtual IDevice* GetDevice() const override = 0;
        virtual IBuffer* GetBuffer() const = 0;
    };
}