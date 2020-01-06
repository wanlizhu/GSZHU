#pragma once

#include "Wanlix/IResource.h"

namespace Wanlix
{
    struct Sampler
    {
        FilterType magFilter = FilterType::Point;
        FilterType minFilter = FilterType::Point;
        FilterType mipFilter = FilterType::Point;
        Uint maxAnisotropy = 1;
        float maxLod = 1000.f;
        float minLod = -1000.f;
        float lodBias = 0.f;
        ComparisonFunc comparisonFunc = ComparisonFunc::Always;
        AddressMode addressU = AddressMode::Wrap;
        AddressMode addressV = AddressMode::Wrap;
        AddressMode addressW = AddressMode::Wrap;
        Float4 borderColor = { 0.f, 0.f, 0.f, 0.f };
    };

    struct SamplerObject : public Sampler
    {
        SamplerHandle apiHandle = nullptr;
    };

    class ITexture : public IResource
    {
        CLASSINFO(ITexture)
        INHERIT_SHARED_FROM_THIS(IResource)
    public:
        virtual ~ITexture() = 0;
        virtual Uint GetMipCount() const = 0;
        virtual Uint GetArraySize() const = 0;
        virtual Uint GetSampleCount() const = 0;
        virtual Uint GetSubresourceArraySlice(Uint subres) const = 0;
        virtual Uint GetSubresourceMipLevel(Uint subres) const = 0;
        virtual Uint GetSubresourceIndex(Uint arraySlice, Uint mipLevel) const = 0;
        virtual Extent GetExtent(Uint mipLevel = 0) const = 0;
        virtual PixelFormat GetPixelFormat() const = 0;
        virtual void GenerateMips() = 0;
        virtual void SaveToFile(Uint arraySlice, Uint mipLevel, String const& filename) const = 0;
    };
}