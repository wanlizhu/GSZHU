#pragma once

#include "RHI/ITexture.h"
#include "RHI/ISampler.h"
#include "RHI/ITextureView.h"
#include "RHI/Class/CTextureBase.h"

namespace Wanlix
{
    template<typename _Interface_>
    class CTextureViewBase : public CDeviceObjectBase<_Interface_>
    {
    public:
        using Texture = typename _Interface_::Texture;
        using Sampler = typename _Interface_::Sampler;

        virtual Texture* GetTexture() const override final { return mTexture; }
        virtual void SetSampler(Sampler* sampler) override final { mSampler = sampler->GetShaderPtr<Sampler>(); }
        virtual Sampler* GetSampler() const override final { return mSampler.get(); }

    protected:
        CTextureViewBase(IDevice* device,
                         const TextureViewDesc& desc,
                         Texture* texture,
                         Bool isDefaultView,
                         const String& name)
            : CDeviceObjectBase<_Interface_>(device, desc, name)
            , mTexture(texture)
            , mTextureStrongRef(isDefaultView ? nullptr : texture->GetSharedPtr<Texture>())
        {}

    protected:
        Texture* mTexture = nullptr;
        SharedPtr<Sampler> mSampler;

    private:
        SharedPtr<Texture> mTextureStrongRef;
    };
}