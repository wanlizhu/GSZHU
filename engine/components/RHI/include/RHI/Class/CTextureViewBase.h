#pragma once

#include "RHI/ITexture.h"
#include "RHI/ISampler.h"
#include "RHI/ITextureView.h"
#include "RHI/Class/CTextureBase.h"

namespace Wanlix
{
    class CTextureViewBase : public CDeviceObjectBase<ITextureView>
    {
    public:
        virtual ITexture* GetTexture() const override final { return mTexture; }
        virtual void SetSampler(ISampler* sampler) override final { mSampler = sampler->GetShaderPtr(); }
        virtual ISampler* GetSampler() const override final { return mSampler.get(); }

    protected:
        CTextureViewBase(IDevice* device,
                         const TextureViewDesc& desc,
                         ITexture* texture,
                         Bool isDefaultView,
                         const String& name)
            : CDeviceObjectBase<ITextureView>(device, desc, name)
            , mTexture(texture)
            , mTextureStrongRef(isDefaultView ? nullptr : texture->GetSharedPtr<ITexture>())
        {}


    protected:
        ITexture* mTexture = nullptr;
        SharedPtr<ISampler> mSampler;

    private:
        SharedPtr<ITexture> mTextureStrongRef;
    };
}