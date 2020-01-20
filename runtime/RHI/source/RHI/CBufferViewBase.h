#pragma once

#include "RHI/IBufferView.h"
#include "CDeviceObjectBase.h"

namespace Wanlix
{
    class CBufferViewBase : public CDeviceObjectBase<IBufferView>
    {
    public:
        virtual IBuffer* GetBuffer() const override final { return mBuffer.get(); }

    protected:
        CBufferViewBase(IDevice* device,
                        const BufferViewDesc& desc,
                        const String& name,
                        Buffer* buffer)
            : CDeviceObjectBase<IBufferView>(device, desc, name)
            , mBuffer(ToPtr<IBuffer>(buffer->GetSharedPtr()))
        {}

    protected:
        SharedPtr<IBuffer> mBuffer;
    };
}