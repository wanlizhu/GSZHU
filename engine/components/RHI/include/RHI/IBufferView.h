#pragma once

#include "RHI/IBuffer.h"
#include "RHI/IResourceView.h"

namespace Wanlix
{
    struct BufferFormat
    {
        EDataType dataType = EDataType::Undefined;
        Uint components = 0;
        Bool isNormalized = false;
    };

    struct BufferViewDesc : public DeviceObjectDesc
    {
        EBufferViewType viewType = EBufferViewType::Undefined;
        BufferFormat format;
        Uint offset = 0;
        Uint size = 0;
    };

    class IBufferView : public IResourceView
    {
    public:
        using Desc = BufferViewDesc;

        virtual IBuffer* GetBuffer() const = 0;
    };
}