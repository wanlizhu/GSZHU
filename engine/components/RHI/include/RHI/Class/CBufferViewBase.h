#pragma once

#include "RHI/IBufferView.h"
#include "RHI/Class/CBufferBase.h"

namespace Wanlix
{
    class CBufferViewBase : public CDeviceObjectBase<IBufferView>
    {
    public:
        virtual IBuffer* GetBuffer() const override final { return mBuffer; }

    protected:
        CBufferViewBase(IDevice* device,
                        const BufferViewDesc& desc,
                        IBuffer* buffer,
                        Bool isDefaultView,
                        const String& name)
            : CDeviceObjectBase<IBufferView>(device, desc, name)
            , mBuffer(buffer)
            , mBufferStrongRef(isDefaultView ? nullptr : buffer->GetSharedPtr<IBuffer>())
        {}

    protected:
        IBuffer* mBuffer = nullptr;

    private:
        SharedPtr<IBuffer> mBufferStrongRef;
    };
}