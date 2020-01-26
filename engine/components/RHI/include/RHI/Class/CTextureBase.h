#pragma once

#include "RHI/ITexture.h"
#include "RHI/Class/CDeviceObjectBase.h"

namespace Wanlix
{
    class CTextureBase : public CDeviceObjectBase<ITexture>
    {
    public:
        virtual SharedPtr<ITextureView> CreateView(const TextureViewDesc& desc) override final;
        virtual SharedPtr<ITextureView> GetDefaultView(ETextureViewType viewType) override final;
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
            : CDeviceObjectBase<ITexture>(device, desc, name)
        {}

        virtual SharedPtr<ITextureView> CreateViewInternal(const TextureViewDesc& viewDesc) = 0;

    protected:
        EResourceState mResourceState = EResourceState::Unknown;
        HashMap<Subresource, EResourceState> mSubresourceStates;
        SharedPtr<ITextureView> mDefaultUAV;
        SharedPtr<ITextureView> mDefaultSRV;
        SharedPtr<ITextureView> mDefaultRTV;
        SharedPtr<ITextureView> mDefaultDSV;
    };
}