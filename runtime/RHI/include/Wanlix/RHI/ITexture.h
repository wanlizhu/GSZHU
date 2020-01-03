#pragma once

#include "Common.h"
#include "IDeviceObject.h"
#include "IBuffer.h"

namespace Wanlix
{
    struct TextureCreateInfo
    {
        String name;
        Usage  usage = Usage::Static;
        Uint   bindFlags = BindFlags::ShaderResource;
        Uint   mipLevels = 0;
        Uint   cpuAccessFlags = CpuAccessFlags::None;
        Bool   srgb = false;
        Bool   generateMips = true;
        PixelFormat format = PixelFormat::Undefined;
    };

    struct TextureDesc : public DeviceObjectDesc
    {
        ResourceType type = ResourceType::Undefined;
        Uint width = 0;
        Uint height = 0;
        union {
            Uint arraySize = 1;
            Uint depth;
        };

        PixelFormat format = PixelFormat::Undefined;
        Uint  mipLevels = 1;
        Usage usage = Usage::Default;
        Uint  bindFlags = BindFlags::None;
        Uint  cpuAccessFlags = CpuAccessFlags::None;
        Uint  miscFlags = MiscFlags::None;
        ClearValue clearValue;
        Uint64 commandQueueMask = 1;
    };

    struct SubresourceData
    {
        const void* data = nullptr;
        IBuffer* srcBuffer;
        Uint srcOffset = 0;
        Uint stride = 0;
        Uint depthStride = 0;
    };

    struct TextureData
    {
        std::vector<SubresourceData> subresources;
    };

    struct MappedSubresource
    {
        Pvoid data = nullptr;
        Uint stride = 0;
        Uint depthStride = 0;
    };

    class RHI_API ITexture : public IResource
    {
        CLASSINFO(ITexture)
        INHERIT_SHARED_FROM_THIS(IResource)
    public:
        /* interfaces inherited from IDeviceObject */
        virtual Pvoid GetNativeHandle() const override = 0;
        virtual TextureDesc const& GetDesc() const override = 0;
        virtual IDevice* GetDevice() const override = 0;

        /* interfaces inherited from IResource */
        virtual Size GetSizeInBytes() const override = 0;
        virtual Uint GetBindFlags() const override = 0;
        virtual ResourceType  GetType() const override = 0;
        virtual ResourceState GetState() const override = 0;
        virtual void SetState(ResourceState state) override = 0;

        virtual ITextureView* CreateView(TextureViewDesc const& desc) = 0;
        virtual ITextureView* GetDefaultView(TextureViewType type) = 0;
    };
}