#pragma once

#include <stdint.h>
#include <vector>
#include <optional>

namespace GS
{
    enum class GIResourceFormat : uint32_t
    {
        Unknown = 0,
        R8Unorm,
        R8Snorm,
        R16Unorm,
        R16Snorm,
        RG8Unorm,
        RG8Snorm,
        RG16Unorm,
        RG16Snorm,
        RGB16Unorm,
        RGB16Snorm,
        R24UnormX8,
        RGB5A1Unorm,
        RGBA8Unorm,
        RGBA8Snorm,
        RGB10A2Unorm,
        RGB10A2Uint,
        RGBA16Unorm,
        RGBA8UnormSrgb,
        R16Float,
        RG16Float,
        RGB16Float,
        RGBA16Float,
        R32Float,
        R32FloatX32,
        RG32Float,
        RGB32Float,
        RGBA32Float,
        R11G11B10Float,
        RGB9E5Float,
        R8Int,
        R8Uint,
        R16Int,
        R16Uint,
        R32Int,
        R32Uint,
        RG8Int,
        RG8Uint,
        RG16Int,
        RG16Uint,
        RG32Int,
        RG32Uint,
        RGB16Int,
        RGB16Uint,
        RGB32Int,
        RGB32Uint,
        RGBA8Int,
        RGBA8Uint,
        RGBA16Int,
        RGBA16Uint,
        RGBA32Int,
        RGBA32Uint,

        BGRA8Unorm,
        BGRA8UnormSrgb,
        
        BGRX8Unorm,
        BGRX8UnormSrgb,
        Alpha8Unorm,
        Alpha32Float,
        R5G6B5Unorm,

        // Depth-stencil
        D32Float,
        D16Unorm,
        D32FloatS8X24,
        D24UnormS8,

        // Compressed formats
        BC1Unorm,   // DXT1
        BC1UnormSrgb, 
        BC2Unorm,   // DXT3
        BC2UnormSrgb,
        BC3Unorm,   // DXT5
        BC3UnormSrgb,
        BC4Unorm,   // RGTC Unsigned Red
        BC4Snorm,   // RGTC Signed Red
        BC5Unorm,   // RGTC Unsigned RG
        BC5Snorm,   // RGTC Signed RG
        BC6HS16,
        BC6HU16,
        BC7Unorm,
        BC7UnormSrgb,
    };

    enum class GIComponentType
    {
        Unknown,        // Unknown format Type
        Float,          // Floating-point formats
        Unorm,          // Unsigned normalized formats
        UnormSrgb,      // Unsigned normalized SRGB formats
        Snorm,          // Signed normalized formats
        Uint,           // Unsigned integer formats
        Sint            // Signed integer formats
    };

    struct GIResourceFormatInfo
    {
        GIResourceFormat format = GIResourceFormat::Unknown;
        const char* name = "";
        uint32_t bytes = 0;
        uint32_t channels = 0;
        GIComponentType componentType = GIComponentType::Unknown;
        struct {
            bool isDepth; 
            bool isStencil;
            bool isCompressed;
        };
        struct { 
            uint32_t width; 
            uint32_t height;
        } compressionRatio;

        static std::optional<GIResourceFormatInfo> Find(GIResourceFormat format);
        static std::optional<GIResourceFormatInfo> Find(const std::string& name);

        static GIResourceFormat SrgbToLinearFormat(GIResourceFormat format);
        static GIResourceFormat LinearToSrgbFormat(GIResourceFormat format);
        
        static bool HasAlpha(GIResourceFormat format);
    };
}