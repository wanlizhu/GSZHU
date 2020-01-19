#pragma once

#include "RHI/IBufferView.h"
#include "TDeviceObjectBase.h"

namespace Wanlix
{
    template<typename _Interface_>
    class TBufferViewBase : public TDeviceObjectBase<_Interface_>
    {
        static_assert(std::is_base_of_v<IBufferView, _Interface_> || std::is_same_v<IBufferView, _Interface_>);
        using BaseClass = TDeviceObjectBase<_Interface_>;
        using DeviceType = typename _Interface_::Device;
        using DescType = typename _Interface_::Desc;

    public:
        virtual IBuffer* GetBuffer() const override final { return mBuffer.get(); }

    protected:
        TBufferViewBase(DeviceType* device,
                        const DescType& desc,
                        const String& name,
                        IBuffer* buffer)
            : BaseClass(device, desc, name)
            , mBuffer(ToPtr<IBuffer>(buffer->GetSharedPtr()))
        {}

    protected:
        SharedPtr<IBuffer> mBuffer;
    };
}