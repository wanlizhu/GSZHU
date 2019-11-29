#pragma once

#include "Types.h"

namespace Wanlix
{
    struct ImageDescriptor
    {
        ImageFormat format = ImageFormat::RGBA;
        DataType    dataType = DataType::UInt8;
        size_t      dataSize = 0;
        void* data = nullptr;
    };

    struct ConstImageDescriptor
    {
        ImageFormat format = ImageFormat::RGBA;
        DataType    dataType = DataType::UInt8;
        size_t      dataSize = 0;
        const void* data = nullptr;
    };

    struct VsyncDescriptor
    {
        bool enabled = false;
        std::uint32_t refreshRate = 60;
        std::uint32_t interval = 1;
    };

    struct VideoModeDescriptor
    {
        Extent resolution;
        int    colorBits = 32;
        int    depthBits = 24;
        int    stencilBits = 8;
        bool   fullscreen = false;
        int    swapChainSize = 2;
    };

    struct SwapChainDescriptor
    {
        VsyncDescriptor     vsync;
        std::uint32_t       samples = 1;
        VideoModeDescriptor videoMode;
    };

    struct ResourceDescriptor
    {
        ResourceType type = ResourceType::Undefined;
        long bindFlags = 0;
        long cpuAccessFlags = 0;
        long miscFlags = 0;
    };

    struct TextureDescriptor : public ResourceDescriptor
    {
        TextureType type = TextureType::Texture2D;
        long        bindFlags = (BindFlags::Sampled | BindFlags::ColorAttachment);
        long        cpuAccessFlags = 0;
        long        miscFlags = (MiscFlags::FixedSamples | MiscFlags::GenerateMips);
        Format      format = Format::RGBA8UNorm;
        Extent      extent = { 1, 1, 1 };
        uint32_t    arrayLayers = 1;
        uint32_t    mipLevels = 0;
        uint32_t    samples = 1;
        Color4F     clearValue;
    };

    struct TextureViewDescriptor
    {
        TextureType type = TextureType::Texture2D;
        Format      format = Format::RGBA8UNorm;
        TextureSubresource subresource;
        TextureSwizzleRGBA swizzle;
    };
}
