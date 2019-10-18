#pragma once

#include <vector>
#include <string>
#include <cassert>
#include "GE2/Utilities.h"

namespace GE2::RHI
{
    enum class EResourceBindFlags : uint32_t
    {
        None = 0x0,             ///< The resource will not be bound the pipeline. Use this to create a staging resource
        Vertex = 0x1,           ///< The resource will be bound as a vertex-buffer
        Index = 0x2,            ///< The resource will be bound as a index-buffer
        Constant = 0x4,         ///< The resource will be bound as a constant-buffer
        StreamOutput = 0x8,     ///< The resource will be bound to the stream-output stage as an output buffer
        ShaderResource = 0x10,  ///< The resource will be bound as a shader-resource
        UnorderedAccess = 0x20, ///< The resource will be bound as an UAV
        RenderTarget = 0x40,    ///< The resource will be bound as a render-target
        DepthStencil = 0x80,    ///< The resource will be bound as a depth-stencil buffer
        IndirectArg = 0x100,    ///< The resource will be bound as an indirect argument buffer
        AccelerationStructure = 0x80000000,  ///< The resource will be bound as an acceleration structure
    };
   
    enum class EResourceFormat : uint32_t
    {
        Unknown,
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
        BC1Unorm,           // DXT1
        BC1UnormSrgb,
        BC2Unorm,           // DXT3
        BC2UnormSrgb,
        BC3Unorm,           // DXT5
        BC3UnormSrgb,
        BC4Unorm,           // RGTC Unsigned Red
        BC4Snorm,           // RGTC Signed Red
        BC5Unorm,           // RGTC Unsigned RG
        BC5Snorm,           // RGTC Signed RG
        BC6HS16,
        BC6HU16,
        BC7Unorm,
        BC7UnormSrgb,

        Count
    };

    GE2_IMPEXP EResourceBindFlags GetFormatBindFlags(EResourceFormat format); // defined in D3D12/Vulkan subdirectory

    enum class EFormatType
    {
        Unknown,        ///< Unknown format Type
        Float,          ///< Floating-point formats
        Unorm,          ///< Unsigned normalized formats
        UnormSrgb,      ///< Unsigned normalized SRGB formats
        Snorm,          ///< Signed normalized formats
        Uint,           ///< Unsigned integer formats
        Sint            ///< Signed integer formats
    };

    struct FormatDesc
    {
        // !!! DO NOT CHANGE THE ORDER OF FIELDS !!!
        EResourceFormat format;
        const std::string name;
        uint32_t bytesPerBlock;
        uint32_t channelCount;
        EFormatType Type;
        struct
        {
            bool isDepth;
            bool isStencil;
            bool isCompressed;
        };
        struct
        {
            uint32_t width;
            uint32_t height;
        } compressionRatio;
    };

    extern const FormatDesc kFormatDescMap[];

    inline uint32_t GetFormatBytesPerBlock(EResourceFormat format)
    {
        assert(kFormatDescMap[(uint32_t)format].format == format);
        return kFormatDescMap[(uint32_t)format].bytesPerBlock;
    }

    inline uint32_t GetFormatPixelsPerBlock(EResourceFormat format)
    {
        assert(kFormatDescMap[(uint32_t)format].format == format);
        return kFormatDescMap[(uint32_t)format].compressionRatio.width * kFormatDescMap[(uint32_t)format].compressionRatio.height;
    }

    inline bool IsDepthFormat(EResourceFormat format)
    {
        assert(kFormatDescMap[(uint32_t)format].format == format);
        return kFormatDescMap[(uint32_t)format].isDepth;
    }

    inline bool IsStencilFormat(EResourceFormat format)
    {
        assert(kFormatDescMap[(uint32_t)format].format == format);
        return kFormatDescMap[(uint32_t)format].isStencil;
    }

    inline bool IsDepthStencilFormat(EResourceFormat format)
    {
        return IsDepthFormat(format) || IsStencilFormat(format);
    }

    inline bool IsCompressedFormat(EResourceFormat format)
    {
        assert(kFormatDescMap[(uint32_t)format].format == format);
        return kFormatDescMap[(uint32_t)format].isCompressed;
    }

    inline uint32_t GetFormatWidthCompressionRatio(EResourceFormat format)
    {
        assert(kFormatDescMap[(uint32_t)format].format == format);
        return kFormatDescMap[(uint32_t)format].compressionRatio.width;
    }

    inline uint32_t GetFormatHeightCompressionRatio(EResourceFormat format)
    {
        assert(kFormatDescMap[(uint32_t)format].format == format);
        return kFormatDescMap[(uint32_t)format].compressionRatio.height;
    }

    inline uint32_t GetFormatChannelCount(EResourceFormat format)
    {
        assert(kFormatDescMap[(uint32_t)format].format == format);
        return kFormatDescMap[(uint32_t)format].channelCount;
    }

    inline EFormatType GetFormatType(EResourceFormat format)
    {
        assert(kFormatDescMap[(uint32_t)format].format == format);
        return kFormatDescMap[(uint32_t)format].Type;
    }

    inline bool IsSrgbFormat(EResourceFormat format)
    {
        return (GetFormatType(format) == EFormatType::UnormSrgb);
    }

    inline EResourceFormat SrgbToLinearFormat(EResourceFormat format)
    {
        switch (format)
        {
        case EResourceFormat::BC1UnormSrgb:
            return EResourceFormat::BC1Unorm;
        case EResourceFormat::BC2UnormSrgb:
            return EResourceFormat::BC2Unorm;
        case EResourceFormat::BC3UnormSrgb:
            return EResourceFormat::BC3Unorm;
        case EResourceFormat::BGRA8UnormSrgb:
            return EResourceFormat::BGRA8Unorm;
        case EResourceFormat::BGRX8UnormSrgb:
            return EResourceFormat::BGRX8Unorm;
        case EResourceFormat::RGBA8UnormSrgb:
            return EResourceFormat::RGBA8Unorm;
        case EResourceFormat::BC7UnormSrgb:
            return EResourceFormat::BC7Unorm;
        default:
            assert(IsSrgbFormat(format) == false);
            return format;
        }
    }

    inline EResourceFormat LinearToSrgbFormat(EResourceFormat format)
    {
        switch (format)
        {
        case EResourceFormat::BC1Unorm:
            return EResourceFormat::BC1UnormSrgb;
        case EResourceFormat::BC2Unorm:
            return EResourceFormat::BC2UnormSrgb;
        case EResourceFormat::BC3Unorm:
            return EResourceFormat::BC3UnormSrgb;
        case EResourceFormat::BGRA8Unorm:
            return EResourceFormat::BGRA8UnormSrgb;
        case EResourceFormat::BGRX8Unorm:
            return EResourceFormat::BGRX8UnormSrgb;
        case EResourceFormat::RGBA8Unorm:
            return EResourceFormat::RGBA8UnormSrgb;
        case EResourceFormat::BC7Unorm:
            return EResourceFormat::BC7UnormSrgb;
        default:
            return format;
        }
    }

    inline EResourceFormat DepthToColorFormat(EResourceFormat format)
    {
        switch (format)
        {
        case EResourceFormat::D16Unorm:
            return EResourceFormat::R16Unorm;
        case EResourceFormat::D24UnormS8:
            return EResourceFormat::R24UnormX8;
        case EResourceFormat::D32Float:
            return EResourceFormat::R32Float;
        case EResourceFormat::D32FloatS8X24:
            assert(false);
            return EResourceFormat::Unknown;
        default:
            assert(IsDepthFormat(format) == false);
            return format;
        }
    }

    inline bool DoesFormatHasAlpha(EResourceFormat format)
    {
        if (GetFormatChannelCount(format) == 4)
        {
            switch (format)
            {
            case EResourceFormat::BGRX8Unorm:
            case EResourceFormat::BGRX8UnormSrgb:
                return false;
            default:
                return true;
            }
        }

        switch (format)
        {
        case EResourceFormat::Alpha32Float:
        case EResourceFormat::Alpha8Unorm:
            return true;
        default:
            return false;
        }
    }

    inline const std::string& to_string(EResourceFormat format)
    {
        assert(kFormatDescMap[(uint32_t)format].format == format);
        return kFormatDescMap[(uint32_t)format].name;
    }

    inline const std::string to_string(EFormatType Type)
    {
        switch (Type)
        {
        case EFormatType::Unknown:   return "Unknown";
        case EFormatType::Float:     return "Float";
        case EFormatType::Unorm:     return "Unorm";
        case EFormatType::UnormSrgb: return "UnormSrgb";
        case EFormatType::Snorm:     return "Snorm";
        case EFormatType::Uint:      return "Uint";
        case EFormatType::Sint:      return "Sint";
        default:
            assert(false);
            return "";
        }
    }
}