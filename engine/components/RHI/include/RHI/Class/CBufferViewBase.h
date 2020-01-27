#pragma once

#include "RHI/IBufferView.h"
#include "RHI/Class/CBufferBase.h"

namespace Wanlix
{
    template<typename _Interface_>
    class CBufferViewBase : public CDeviceObjectBase<_Interface_>
    {
        static_assert(std::is_base_of_v<IBufferView, _Interface_> || std::is_same_v<IBufferView, _Interface_>);
    public:
        using Buffer = typename _Interface_::Buffer;

        virtual Buffer* GetBuffer() const override final { return mBuffer; }

    protected:
        CBufferViewBase(IDevice* device,
                        const BufferViewDesc& desc,
                        Buffer* buffer,
                        Bool isDefaultView,
                        const String& name)
            : CDeviceObjectBase<_Interface_>(device, desc, name)
            , mBuffer(buffer)
            , mBufferStrongRef(isDefaultView ? nullptr : buffer->GetSharedPtr<Buffer>())
        {}

    protected:
        Buffer* mBuffer = nullptr;

    private:
        SharedPtr<Buffer> mBufferStrongRef;
    };
}