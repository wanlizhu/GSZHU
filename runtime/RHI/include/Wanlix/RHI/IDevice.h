#pragma once

#include "Wanlix/Object.h"

namespace Wanlix
{
    class RHI_API IDevice : public Object
    {
        CLASSINFO(IDevice)
        INHERIT_SHARED_FROM_THIS(Object)
    public:
        virtual ~IDevice() = default;
        virtual IDeviceContext* Context() const = 0;
        virtual IBuffer* CreateBuffer(BufferDesc const& desc, BufferData const* data) = 0;
        virtual IShader* CreateShader(ShaderCreateInfo const& info) = 0;
        virtual ITexture* CreateTexture(TextureDesc const& desc, TextureData const* data) = 0;
        virtual ISampler* CreateSampler(SamplerDesc const& desc) = 0;
        virtual IResourceMapping* CreateResourceMapping(ResourceMappingDesc const& desc) = 0;
        virtual IPipelineState* CreatePipelineState(PipelineStateDesc const& desc) = 0;
        virtual IFence* CreateFence(FenceDesc const& desc) = 0;
        virtual DeviceCapability const& Capability() const = 0;
        virtual void ReleaseResources(Bool forceRelease = false) = 0;
    };
}