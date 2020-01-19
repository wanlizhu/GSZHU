#pragma once

namespace Wanlix
{
    struct BlendStateDesc;
    struct DepthStencilStateDesc;
    struct RasterizerStateDesc;

    class IDevice;
    class IDeviceObject;
    struct DeviceObjectDesc;

    class IResource;
    class IResourceView;
    class IResourceTable;
    struct ResourceDesc;
    struct ResourceViewDesc;

    class IBuffer;
    class IBufferView;
    struct BufferDesc;
    struct BufferViewDesc;

    class ITexture;
    class ITextureView;
    struct TextureDesc;
    struct TextureViewDesc;

    class IShader;
    class IShaderVariable;
    class IShaderResourceBinding;
    class IPipelineState;
    struct ShaderDesc;
    struct ShaderCreateInfo;
    struct PipelineStateDesc;

    class ISampler;
    class ISwapChain;
    class ICommandList;
    class IFence;
    struct SamplerDesc;
    struct SwapChainDesc;
    struct CommandListDesc;
    struct FenceDesc;
    
    class IDeviceContext;
    class ICopyContext;
    class IComputeContext;
    class IDrawContext;
}