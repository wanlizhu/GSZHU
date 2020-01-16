#pragma once

#include "Graphics/RHI/IResource.h"
#include "Graphics/RHI/IResourceView.h"

namespace Wanlix
{
    struct ClearValue
    {
        Optional<Float4> color = std::nullopt;
        Optional<float> depth = std::nullopt;
        Optional<Uint8> stencil = std::nullopt;
    };

    struct TextureDesc : public ResourceDesc
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
    };

    struct Subresource
    {
        Uint slice = 0;
        Uint sliceCount = 1;
        Uint mipLevel = 0;
        Uint mipLevelCount = 1;

        Subresource() = default;
        Subresource(Uint slice, Uint mipLevel)
            : slice(slice)
            , sliceCount(1)
            , mipLevel(mipLevel)
            , mipLevelCount(1)
        {}
        Subresource(Uint slice, Uint sliceCount, Uint mipLevel, Uint mipLevelCount)
            : slice(slice)
            , sliceCount(sliceCount)
            , mipLevel(mipLevel)
            , mipLevelCount(mipLevelCount)
        {}
        inline bool operator==(const Subresource& rhs) const
        {
            return slice == rhs.slice
                && sliceCount == rhs.sliceCount
                && mipLevel == rhs.mipLevel
                && mipLevelCount == rhs.mipLevelCount;
        }
    };

    class Sampler;
    class ITextureView;
    struct TextureViewDesc;

    class ITexture : public IResource
    {
    public:
        virtual void GenerateMips() const = 0;
        virtual void SaveToFile(Uint mipLevel, Uint slice, const String& filename) = 0;
        virtual const String& GetSourceFile() const = 0;
        virtual Uint GetSubresourceIndex(const Subresource& subres) const = 0;
        virtual SharedPtr<ITextureView> CreateView(const TextureViewDesc& desc) = 0;
        virtual Optional<EResourceState> GetState() const = 0;
        virtual Optional<EResourceState> GetSubresourceState(const Subresource& subres) const = 0;

        // Interfaces inherited from IResource
        virtual EResourceType GetType() const = 0;
        
        // Interfaces inherited from IDeviceObject
        virtual void SetName(const String& name) = 0;
        virtual const String& GetName() const = 0;
        virtual Handle ApiHandle() const = 0;
        virtual const TextureDesc& GetDesc() const = 0;
    };
}