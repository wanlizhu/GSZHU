#pragma once

namespace GS2::RHI
{
    struct VertexInputState;
    struct InputAssemblyState;
    struct RasterizationState;
    struct TessellationState;
    struct ViewportScissorState;
    struct MultisampleState;
    struct ColorBlendState;
    struct DepthStencilState;
    struct DynamicState;
    struct Sampler;

    class CopyContext;
    class ComputeContext;
    class DrawContext;
    class ResourceManager;
    class Window;

    class IDevice;
    class IDeviceObject;
    class IDeviceTimer;
    class IFence;
    class ISemaphore;
    class IResource;
    class IResourceView;
    class IResourceAllocator;
    class IBuffer;
    class IBufferView;
    class ITexture;
    class ITextureView;
    class ICommandBuffer;
    class IDescriptorPool;
    class IDescriptorSet;
    class IDescriptorSetLayout;
    class IPipeline;
    class IPipelineLayout;
    class IShader;
    class IRenderPass;
    class IFramebuffer;
    class IFramebufferAttachment;
    class ISurface;
    class ISwapchain;
    class IQueryPool;
    class IEvent;
}