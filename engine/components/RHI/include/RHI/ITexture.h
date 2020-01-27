#pragma once

#include "RHI/IResource.h"
#include "RHI/IResourceView.h"

namespace Wanlix
{
    class ITextureView;
    struct TextureViewDesc;

    struct ClearValue
    {
        Optional<Float4> color = std::nullopt;
        Optional<float> depth = std::nullopt;
        Optional<Uint8> stencil = std::nullopt;
    };

    struct Subresource
    {
        Uint mipLevel = 0;
        Uint slice = 0;
        Optional<Region> region = std::nullopt;

        Subresource() = default;
        Subresource(Uint mipLevel, Uint slice)
            : mipLevel(mipLevel)
            , slice(slice)
        {}
        inline Uint64 Hash() const
        {
            Uint64 const h1(std::hash<Uint>{}(mipLevel));
            Uint64 const h2(std::hash<Uint>{}(slice));
            Uint64 const h3(region.has_value() ? region.value().Hash() : 1);
            return h1 ^ ((h2 ^ (h3 << 1)) << 2);
        }
    };

    struct TextureDesc : public DeviceObjectDesc
    {
        EResourceType textureType = EResourceType::Undefined;
        Uint width = 0;
        Uint height = 0;
        Uint depth = 0;
        EPixelFormat pixelFormat = EPixelFormat::Undefined;
        Uint mipLevels = 1;
        Uint sampleCount = 1;
        EUsage usage = EUsage::Default;
        EBindFlags bindFlags = EBindFlags::None;
        ECPUAccessFlags cpuAccessFlags = ECPUAccessFlags::None;
        EMiscFlags miscFlags = EMiscFlags::None;
        ClearValue clearValue;
        Uint64 queueMask = 1;
        EResourceState initialState = EResourceState::Common;
    };

    class ITexture : public IResource
    {
    public:
        virtual SharedPtr<ITextureView> CreateView(const TextureViewDesc& desc) = 0;
        virtual SharedPtr<ITextureView> GetDefaultView(ETextureViewType viewType) = 0;
        virtual void SetState(EResourceState state) = 0;
        virtual void SetSubresourceState(Uint mipLevel, Uint slice, EResourceState state) = 0;
        virtual Optional<EResourceState> GetState() const = 0;
        virtual Optional<EResourceState> GetSubresourceState(Uint mipLevel, Uint slice) const = 0;
    
        virtual void GenerateMips() const = 0;
        virtual void SaveToFile(Uint mipLevel, Uint slice, const String& filename) = 0;
        virtual const String& GetSourceFile() const = 0;
    };
}

namespace std
{
    template<> struct hash<Wanlix::Subresource>
    {
        typedef Wanlix::Subresource argument_type;
        typedef std::size_t result_type;

        result_type operator()(argument_type const& subres) const
        {
            return subres.Hash();
        }
    };
}