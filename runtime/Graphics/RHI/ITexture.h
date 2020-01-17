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

    struct Subresource
    {
        Uint mipLevel = 0;
        Uint slice = 0;
        Optional<Region> region = std::nullopt;
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

    class ITexture : public IResource
    {
    public:
        virtual void GenerateMips() const = 0;
        virtual void SaveToFile(Uint mipLevel, Uint slice, const String& filename) = 0;
        virtual const String& GetSourceFile() const = 0;
        virtual SharedPtr<ITextureView> CreateView(const TextureViewDesc& desc) = 0;
        virtual Optional<EResourceState> GetState() const = 0;
        virtual Optional<EResourceState> GetSubresourceState(Uint mipLevel, Uint slice) const = 0;

        // Interfaces inherited from IResource
        virtual EResourceType GetType() const = 0;
        
        // Interfaces inherited from IDeviceObject
        virtual void SetName(const String& name) = 0;
        virtual const String& GetName() const = 0;
        virtual Handle ApiHandle() const = 0;
        virtual const TextureDesc& GetDesc() const = 0;
    };
}