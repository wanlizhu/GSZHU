#pragma once

#include "RHI/ITexture.h"
#include "RHI/IResourceView.h"

namespace Wanlix
{
    struct TextureViewDesc : public DeviceObjectDesc
    {
        ETextureViewType viewType = ETextureViewType::Undefined;
        EResourceType textureType = EResourceType::Undefined;
        EPixelFormat pixelFormat = EPixelFormat::Undefined;
        Uint firstMipLevel = 0;
        Uint mipLevelCount = 0;
        Uint firstSlice = 0;
        Uint sliceCount = 0;
        Uint accessFlags = 0;
    };

    class ISampler;

    class ITextureView : public IResourceView
    {
    public:
        using Desc = TextureViewDesc;

        virtual ITexture* GetTexture() const = 0;
        virtual void SetSampler(ISampler* sampler) = 0;
        virtual ISampler* GetSampler() const = 0;
    };
}