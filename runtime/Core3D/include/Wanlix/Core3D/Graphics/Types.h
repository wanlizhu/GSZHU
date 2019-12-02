#pragma once

#include "Wanlix/Core3D/Types.h"
#include "Config.h"
#include "Color.h"
#include "Flags.h"
#include "Format.h"
#include "Limits.h"

namespace Wanlix
{
    using ByteBuffer = std::unique_ptr<char[]>;

    /* Only supported with: Vulkan. */
    struct ApplicationDescriptor
    {
        String   applicationName;
        uint32_t applicationVersion;
        String   engineName;
        uint32_t engineVersion;
    };

    struct VulkanDeviceConfig
    {
        ApplicationDescriptor application;
        StringList enabledLayers;
        uint64_t   minDeviceMemoryAllocationSize = 1024 * 1024;
        bool       reduceDeviceMemoryFragmentation = false;
    };

    struct OpenGLDeviceConfig
    {
        OpenGLContextProfile contextProfile = OpenGLContextProfile::CoreProfile;
        int  majorVersion = 0;
        int  minorVersion = 0;
        bool originLowerLeft = false; //!< Upper-left is the default of Direct3D and Vulkan.
        bool invertFrontFace = false;
    };

    struct TextureSwizzleRGBA
    {
        TextureSwizzle r = TextureSwizzle::Red;
        TextureSwizzle g = TextureSwizzle::Green;
        TextureSwizzle b = TextureSwizzle::Blue;
        TextureSwizzle a = TextureSwizzle::Alpha;
    };

    struct TextureSubresource
    {
        uint32_t baseArrayLayer = 0; 
        uint32_t numArrayLayers = 1; 
        uint32_t baseMipLevel = 0;
        uint32_t numMipLevels = 1;
    };

    struct TextureLocation
    {
        Offset   offset;
        uint32_t arrayLayer = 0;
        uint32_t mipLevel = 0;
    };

    struct TextureRegion
    {
        TextureSubresource subresource;
        Offset offset;
        Extent extent;
    };

    struct SubresourceLayout
    {
        uint32_t rowStride = 0; // Bytes per row
        uint32_t layerStride = 0; // Bytes per layer
        uint32_t dataSize = 0; // Bytes per resource
    };

    struct Viewport
    {
        float x = 0.0f;
        float y = 0.0f;
        float width = 0.0f;
        float height = 0.0f;
        float minDepth = 0.0f;
        float maxDepth = 1.0f;
    };

    struct Scissor
    {
        int32_t x = 0; 
        int32_t y = 0; 
        int32_t width = 0; 
        int32_t height = 0;
    };

    struct VertexAttribute
    {
        std::string name; 
        Format      format = Format::RGBA32Float;
        uint32_t    location = 0; 
        uint32_t    semanticIndex = 0;
        SystemValue systemValue = SystemValue::Undefined;
        uint32_t    slot = 0;
        uint32_t    offset = 0;
        uint32_t    stride = 0;
        uint32_t    instanceDivisor = 0;
    };
}