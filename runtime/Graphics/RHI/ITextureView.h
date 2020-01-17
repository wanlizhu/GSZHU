#pragma once

#include "Graphics/RHI/ITexture.h"
#include "Graphics/RHI/IResourceView.h"

namespace Wanlix
{
    struct TextureViewDesc : public ResourceViewDesc
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
        virtual void SetSampler(ISampler* sampler) = 0;
        virtual ISampler* GetSampler() const = 0;

        // Interfaces inherited from IResourceView
        virtual ITexture* GetResource() const = 0;

        // Interfaces inherited from IDeviceObject
        virtual void SetName(const String& name) = 0;
        virtual const String& GetName() const = 0;
        virtual Handle ApiHandle() const = 0;
        virtual const TextureViewDesc& GetDesc() const = 0;
    };
}