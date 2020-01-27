#pragma once

#include "RHI/IBuffer.h"
#include "RHI/Class/CDeviceObjectBase.h"

namespace Wanlix
{
    template<typename _Interface_>
    class CBufferBase : public CDeviceObjectBase<_Interface_>
    {
        static_assert(std::is_base_of_v<IBuffer, _Interface_> || std::is_same_v<IBuffer, _Interface_>);
    public:
        using View = typename _Interface_::View;

        virtual SharedPtr<View> CreateView(const BufferViewDesc& desc) override final;
        virtual SharedPtr<View> GetDefaultView(EBufferViewType viewType) override final;
        virtual void SetState(EResourceState state) override final;
        virtual EResourceState GetState() const override final;
        
        Bool IsInKnownState() const;
        Bool CheckState(EResourceState state) const;

    protected:
        CBufferBase(IDevice* device,
                    const BufferDesc& desc,
                    const String& name)
            : CDeviceObjectBase<_Interface_>(device, desc, name)
        {}

        virtual SharedPtr<View> CreateViewInternal(const BufferViewDesc& viewDesc, Bool isDefault) = 0;
        void AdjustBufferViewDesc(BufferViewDesc& desc) const;

    protected:
        EResourceState mResourceState = EResourceState::Unknown;
        SharedPtr<View> mDefaultUAV;
        SharedPtr<View> mDefaultSRV;
    };
}