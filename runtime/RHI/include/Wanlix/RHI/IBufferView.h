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

    class IBufferView : public inherit_shared_from_this<IDeviceObject, IBufferView>
    {
    public:
        virtual ~IBufferView() = default;

        /* interfaces inherited from IDeviceObject */
        virtual Pvoid GetNativeHandle() const override = 0;
        virtual BufferViewDesc const& GetDesc() const override = 0;
        virtual IDevice* GetDevice() const override = 0;

        virtual IBuffer* GetBuffer() const = 0;
    };
}