#pragma once

#include <GSZHU/ENUM/ETextureViewType.h>
#include <GSZHU/DESC/STextureViewDesc.h>
#include <GSZHU/IDeviceObject.h>

namespace GSZHU {
    // To create a texture view, call ITexture::CreateView().
    // Texture view holds strong references to the texture.
    // The texture view will also keep a strong reference to the texture sampler, if any is set.
    class ITextureView : public IDeviceObject {
    public:
        using DESC = STextureDesc;

        virtual const STextureViewDesc& GetDesc()const = 0;
        virtual void SetSampler(class ISampler *pSampler) = 0;
        virtual ISampler* GetSampler() = 0;
        virtual class ITexture* GetTexture() = 0;
    };
}