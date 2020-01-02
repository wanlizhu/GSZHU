#pragma once

#include <cstdint>
#include <array>
#include <type_traits>
#include <cassert>
#include <algorithm>
#include <unordered_map>
#include <memory>
#include <typeinfo>
#include <typeindex>
#include <functional>
#include <string>
#include <vector>
#include <queue>
#include <deque>
#include <list>
#include <codecvt>
#include <chrono>
#include <filesystem>
#include <fstream>
#include <sstream>
#include <iostream>
#include <any>
#include <tuple>
#include <variant>
#include <thread>
#include <mutex>
#include <condition_variable>
#include "Wanlix/Common.h"
#include "Config.h"
#include "Format.h"
#include "Flags.h"
#include "IAllocator.h"
#include "NonCopyable.h"

namespace Wanlix
{
    class IDevice;
    class IDeviceContext;
    class IDeviceObject;
    class IResource;
    class IBuffer;
    class IBufferView;
    class IShader;
    class IShaderVariable;
    class ITexture;
    class ITextureView;
    class ISampler;
    class ISwapChain;
    class IResourceBinding;
    class IResourceMapping;
    class IPipelineState;
    class IFence;
    class ICommandBuffer;
    struct BufferDesc;
    struct BufferViewDesc;
    struct ShaderDesc;
    struct ShaderCreateInfo;
    struct TextureDesc;
    struct TextureViewDesc;
    struct SamplerDesc;
    struct FenceDesc;
    struct CommandBufferDesc;
    struct ResourceMappingDesc;
    struct PipelineStateDesc;
    struct BlendStateDesc;
    struct DepthStencilStateDesc;
    struct RasterizerStateDesc;
    struct DeviceCapability;
    struct InputLayoutDesc;
    struct SwapChainDesc;

    struct Range
    {
        Uint offset = 0;
        Uint size = 0;
    };

    struct Offset
    {
        Uint x = 0;
        Uint y = 0;
        Uint z = 0;
    };

    struct Extent
    {
        Uint width = 0;
        Uint height = 0;
        Uint depth = 1;
    };

    struct Region
    {
        Offset offset;
        Extent extent;
    };

    struct Rect
    {
        Int left = 0;
        Int top = 0;
        Int right = 0;
        Int bottom = 0;
    };

    struct Subresource
    {
        Uint baseArrayLayer = 0;
        Uint numArrayLayers = 1;
        Uint baseMipLevel = 0;
        Uint numMipLevels = 1;
    };

    struct SwizzleRGBA
    {
        Byte r = 0;
        Byte g = 1;
        Byte b = 2;
        Byte a = 3;
    };

    struct Color4i
    {
        union {
            struct { Byte r, g, b, a; };
            Byte data[4];
            Int packed;
        };

        Color4i();
        Color4i(Int val);
        Color4i(Int r, Int g, Int b, Int a = 255);
        Color4i(std::initializer_list<Int> init);
        Color4i(Color4f const& rhs);
        Color4i& operator=(Color4f const& rhs);

        Byte&       operator[](Int i);
        Byte const& operator[](Int i) const;
        Bool operator==(const Color4i& rhs) const;
        Bool operator!=(const Color4i& rhs) const;
    };

    struct Color4f
    {
        union {
            struct { float r, g, b, a; };
            float data[4];
        };

        Color4f();
        Color4f(float val);
        Color4f(float r, float g, float b, float a = 1.f);
        Color4f(std::initializer_list<float> init);
        Color4f(Color4i const& rhs);
        Color4f& operator=(Color4i const& rhs);

        float& operator[](Int i);
        float const& operator[](Int i) const;
        Bool operator==(const Color4f& rhs) const;
        Bool operator!=(const Color4f& rhs) const;
    };

    struct Viewport
    {
        float xTL = 0.f;
        float yTL = 0.f;
        float width = 0.f;
        float height = 0.f;
        float minDepth = 0.f;
        float maxDepth = 1.f;
    };

    struct ClearValue
    {
        PixelFormat format = PixelFormat::Undefined;
        Color4f color;
        float depth = 0.f;
        Uint stencil = 0;
    };

    struct StateTransitionDesc
    {
        ITexture* texture = nullptr;
        IBuffer*  buffer = nullptr;
        Uint mipLevels = 0;
        Uint firstArraySlice = 0;
        Uint numArraySlices = 0;
        ResourceState oldState = ResourceState::Unknown;
        ResourceState newState = ResourceState::Unknown;
        TransitionBarrierType barrierType = TransitionBarrierType::Immediate;
        Bool updateResourceState = false;
    };  
}