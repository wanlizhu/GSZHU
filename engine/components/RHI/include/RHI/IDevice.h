#pragma once

#include "Wanlix/Common.h"
#include "Wanlix/Window.h"
#include "RHI/Constants.h"
#include "RHI/PixelFormat.h"


namespace Wanlix
{
    class IBuffer;
    class ITexture;
    class IShader;
    class ISampler;
    class IPipeline;
    class IFence;

    struct BufferDesc;
    struct TextureDesc;
    struct ShaderDesc;
    struct SamplerDesc;
    struct PipelineDesc;
    struct FenceDesc;

    struct DeviceDesc
    {
        EPixelFormat colorFormat = EPixelFormat::RGBA8UNorm_sRGB;
        EPixelFormat depthFormat = EPixelFormat::D32Float;
        Uint majorVersion = 0;
        Uint minorVersion = 0;
        Bool enableVsync = false;
        Bool enableDebugLayer = DEFAULT_ENABLE_DEBUG_LAYER;
        Uint commandQueueNums[(int)ECommandQueue::Count] = { 0, 0, 1 };
    };

    class IDevice : public std::enable_shared_from_this<IDevice>
    {
    public:
        using Desc = DeviceDesc;

        virtual SharedPtr<IDevice> GetSharedPtr() final { return shared_from_this(); };
        virtual SharedPtr<IBuffer> CreateBuffer(const BufferDesc& desc, const void* data = nullptr) = 0;
        virtual SharedPtr<IShader> CreateShader(const ShaderCreateInfo& info) = 0;
        virtual SharedPtr<ITexture> CreateTexture(const TextureDesc& desc, const void* data = nullptr) = 0;
        virtual SharedPtr<ISampler> CreateSampler(const SamplerDesc& desc) = 0;
        virtual SharedPtr<IPipeline> CreatePipeline(const PipelineDesc& desc) = 0;
        virtual SharedPtr<IFence> CreateFence(const FenceDesc& desc) = 0;
        virtual void ReleaseResources(Bool forceRelease = false) = 0;
    };
}