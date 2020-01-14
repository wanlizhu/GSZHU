#pragma once

#include "Core/Common.h"
#include "Core/Window.h"
#include "RHI/PixelFormat.h"
#include "RHI/BasicTypes.h"

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
#ifdef ENABLE_D3D12
        Array<UUID> features;
#endif
    };

    class DLL_DECL IDevice
    {
    public:
        static SharedPtr<IDevice> Create(const DeviceDesc& desc, SharedPtr<Window> window);

        virtual void Destroy() = 0;
        virtual void EnableVsync(Bool flag) = 0;
        virtual Bool IsWindowOccluded() const = 0;
        virtual IDrawContext* GetDrawContext() const = 0;
        virtual IFramebuffer* GetFramebuffer() const = 0;
        virtual ICommandQueue* GetCommandQueue(ECommandQueue type, Uint index) const = 0;
        virtual Handle ApiHandle() const = 0;
        virtual void Present() = 0;
        virtual void Flush(Bool wait = false) = 0;
        virtual Bool IsVsyncEnabled() const = 0;
        virtual void Resize(Uint width, Uint height) = 0;
        virtual DeviceDesc& GetDesc() const = 0;
        virtual IDescriptorPool* GetCPUDescriptorPool() const = 0;
        virtual IDescriptorPool* GetGPUDescriptorPool() const = 0;
        virtual Bool IsFeatureSupported(ESupportedFeatures flags) const = 0;
    };
}