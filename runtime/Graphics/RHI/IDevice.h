#pragma once

#include "Core/Common.h"
#include "Core/Window.h"
#include "Graphics/RHI/PixelFormat.h"
#include "Graphics/RHI/ForwardDecl.h"

namespace Wanlix
{
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

    class IDevice
    {
    public:
        virtual SharedPtr<IBuffer> CreateBuffer(const BufferDesc& desc, const void* data = nullptr) = 0;
        virtual SharedPtr<IShader> CreateShader(const ShaderCreateInfo& info) = 0;
        virtual SharedPtr<ITexture> CreateTexture(const TextureDesc& desc, const void* data = nullptr) = 0;
        virtual SharedPtr<ISampler> CreateSampler(const SamplerDesc& desc) = 0;
        virtual SharedPtr<IPipelineState> CreatePipelineState(const PipelineStateDesc& desc) = 0;
        virtual SharedPtr<IFence> CreateFence(const FenceDesc& desc) = 0;
        virtual void ReleaseResources(Bool forceRelease = false) = 0;
    };
}