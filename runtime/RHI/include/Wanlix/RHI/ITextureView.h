#pragma once

#include "ITexture.h"

namespace Wanlix
{
    struct TextureViewDesc : public DeviceObjectDesc
    {
        TextureViewType viewType = TextureViewType::Undefined;
        ResourceType textureType = ResourceType::Undefined;
        PixelFormat format = PixelFormat::Undefined;
        Uint mostDetailedMip = 0;
        Uint mipLevels = 0;
        union {
            Uint firstArraySlice = 0;
            Uint firstDepthSlice;
        };
        union {
            Uint numArraySlices = 0;
            Uint numDepthSlices;
        };
        Uint accessFlags = UavAccessFlags::Undefined;
        Uint flags = TextureViewFlags::None;
    };

    class RHI_API ITextureView : public IDeviceObject
    {
        CLASSINFO(ITextureView)
        INHERIT_SHARED_FROM_THIS(IDeviceObject)
    public:
        /* interfaces inherited from IDeviceObject */
        virtual Pvoid GetNativeHandle() const override = 0;
        virtual TextureViewDesc const& GetDesc() const override = 0;
        virtual IDevice* GetDevice() const override = 0;

        virtual void SetSampler(ISampler* sampler) = 0;
        virtual ISampler* GetSampler() = 0;
        virtual ITexture* GetTexture() = 0;
    };
}