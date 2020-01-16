#pragma once

#include "Graphics/RHI/IBuffer.h"
#include "Graphics/RHI/IResourceView.h"

namespace Wanlix
{
    struct BufferFormat
    {
        EDataType dataType = EDataType::Undefined;
        Uint components = 0;
        Bool isNormalized = false;
    };

    struct BufferViewDesc : public ResourceViewDesc
    {
        EBufferViewType viewType = EBufferViewType::Undefined;
        BufferFormat format;
        Uint offset = 0;
        Uint size = 0;
    };

    class IBufferView : public IResourceView
    {
    public:
        // Interfaces inherited from IResourceView
        virtual IBuffer* GetResource() const = 0;

        // Interfaces inherited from IDeviceObject
        virtual void SetName(const String& name) = 0;
        virtual const String& GetName() const = 0;
        virtual Handle ApiHandle() const = 0;
        virtual const BufferViewDesc& GetDesc() const = 0;
    };
}