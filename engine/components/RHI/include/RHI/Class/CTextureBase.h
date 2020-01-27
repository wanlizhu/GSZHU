#pragma once

#include "RHI/ITexture.h"
#include "RHI/Class/CDeviceObjectBase.h"

namespace Wanlix
{
    template<typename _Interface_>
    class CTextureBase : public CDeviceObjectBase<_Interface_>
    {
    public:
        using View = typename _Interface_::View;

        virtual SharedPtr<View> CreateView(const TextureViewDesc& desc) override final;
        virtual SharedPtr<View> GetDefaultView(ETextureViewType viewType) override final;
        virtual void SetState(EResourceState state) override final;
        virtual void SetSubresourceState(Uint mipLevel, Uint slice, EResourceState state) override final;
        virtual Optional<EResourceState> GetState() const override final;
        virtual Optional<EResourceState> GetSubresourceState(Uint mipLevel, Uint slice) const override final;

        Bool IsInKnownState() const;
        Bool CheckState(EResourceState state) const;

    protected:
        CTextureBase(IDevice* device,
                     const TextureDesc& desc,
                     const String& name)
            : CDeviceObjectBase<_Interface_>(device, desc, name)
        {}

        virtual SharedPtr<View> CreateViewInternal(const TextureViewDesc& viewDesc) = 0;

    protected:
        EResourceState mResourceState = EResourceState::Unknown;
        HashMap<Subresource, EResourceState> mSubresourceStates;
        SharedPtr<View> mDefaultUAV;
        SharedPtr<View> mDefaultSRV;
        SharedPtr<View> mDefaultRTV;
        SharedPtr<View> mDefaultDSV;
    };
}