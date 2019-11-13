#include "GS/GIDevice/EResourceFormat.h"
#include <algorithm>
#include <vector>

namespace GS::GI
{
    const std::vector<ResourceFormatInfo> kResourceFormatInfoList = 
    {
        // Format                           Name,           BytesPerBlock ChannelCount  Type                        {bDepth,   bStencil, bCompressed},   {CompressionRatio.Width,     CompressionRatio.Height}
        { EResourceFormat::Unknown,          "Unknown",         0,              0,      EComponentType::Unknown,    {false,  false, false,},             {1, 1}},
        { EResourceFormat::R8Unorm,          "R8Unorm",         1,              1,      EComponentType::Unorm,      {false,  false, false,},             {1, 1}},
        { EResourceFormat::R8Snorm,          "R8Snorm",         1,              1,      EComponentType::Snorm,      {false,  false, false,},             {1, 1}},
        { EResourceFormat::R16Unorm,         "R16Unorm",        2,              1,      EComponentType::Unorm,      {false,  false, false,},             {1, 1}},
        { EResourceFormat::R16Snorm,         "R16Snorm",        2,              1,      EComponentType::Snorm,      {false,  false, false,},             {1, 1}},
        { EResourceFormat::RG8Unorm,         "RG8Unorm",        2,              2,      EComponentType::Unorm,      {false,  false, false,},             {1, 1}},
        { EResourceFormat::RG8Snorm,         "RG8Snorm",        2,              2,      EComponentType::Snorm,      {false,  false, false,},             {1, 1}},
        { EResourceFormat::RG16Unorm,        "RG16Unorm",       4,              2,      EComponentType::Unorm,      {false,  false, false,},             {1, 1}},
        { EResourceFormat::RG16Snorm,        "RG16Snorm",       4,              2,      EComponentType::Snorm,      {false,  false, false,},             {1, 1}},
        { EResourceFormat::RGB16Unorm,       "RGB16Unorm",      6,              3,      EComponentType::Unorm,      {false,  false, false,},             {1, 1}},
        { EResourceFormat::RGB16Snorm,       "RGB16Snorm",      6,              3,      EComponentType::Snorm,      {false,  false, false,},             {1, 1}},
        { EResourceFormat::R24UnormX8,       "R24UnormX8",      4,              1,      EComponentType::Unorm,      {false,  false, false,},             {1, 1}},
        { EResourceFormat::RGB5A1Unorm,      "RGB5A1Unorm",     2,              4,      EComponentType::Unorm,      {false,  false, false,},             {1, 1}},
        { EResourceFormat::RGBA8Unorm,       "RGBA8Unorm",      4,              4,      EComponentType::Unorm,      {false,  false, false,},             {1, 1}},
        { EResourceFormat::RGBA8Snorm,       "RGBA8Snorm",      4,              4,      EComponentType::Snorm,      {false,  false, false,},             {1, 1}},
        { EResourceFormat::RGB10A2Unorm,     "RGB10A2Unorm",    4,              4,      EComponentType::Unorm,      {false,  false, false,},             {1, 1}},
        { EResourceFormat::RGB10A2Uint,      "RGB10A2Uint",     4,              4,      EComponentType::Uint,       {false,  false, false,},             {1, 1}},
        { EResourceFormat::RGBA16Unorm,      "RGBA16Unorm",     8,              4,      EComponentType::Unorm,      {false,  false, false,},             {1, 1}},
        { EResourceFormat::RGBA8UnormSrgb,   "RGBA8UnormSrgb",  4,              4,      EComponentType::UnormSrgb,  {false,  false, false,},             {1, 1}},
        // Format                           Name,           BytesPerBlock ChannelCount  Type                        {bDepth,   bStencil, bCompressed},   {CompressionRatio.Width,     CompressionRatio.Height}
        { EResourceFormat::R16Float,         "R16Float",        2,              1,      EComponentType::Float,      {false,  false, false,},             {1, 1}},
        { EResourceFormat::RG16Float,        "RG16Float",       4,              2,      EComponentType::Float,      {false,  false, false,},             {1, 1}},
        { EResourceFormat::RGB16Float,       "RGB16Float",      6,              3,      EComponentType::Float,      {false,  false, false,},             {1, 1}},
        { EResourceFormat::RGBA16Float,      "RGBA16Float",     8,              4,      EComponentType::Float,      {false,  false, false,},             {1, 1}},
        { EResourceFormat::R32Float,         "R32Float",        4,              1,      EComponentType::Float,      {false,  false, false,},             {1, 1}},
        { EResourceFormat::R32FloatX32,      "R32FloatX32",     4,              1,      EComponentType::Float,      {false,  false, false,},             {1, 1}},
        { EResourceFormat::RG32Float,        "RG32Float",       8,              2,      EComponentType::Float,      {false,  false, false,},             {1, 1}},
        { EResourceFormat::RGB32Float,       "RGB32Float",      12,             3,      EComponentType::Float,      {false,  false, false,},             {1, 1}},
        { EResourceFormat::RGBA32Float,      "RGBA32Float",     16,             4,      EComponentType::Float,      {false,  false, false,},             {1, 1}},
        { EResourceFormat::R11G11B10Float,   "R11G11B10Float",  4,              3,      EComponentType::Float,      {false,  false, false,},             {1, 1}},
        { EResourceFormat::RGB9E5Float,      "RGB9E5Float",     4,              3,      EComponentType::Float,      {false,  false, false,},             {1, 1}},
        { EResourceFormat::R8Int,            "R8Int",           1,              1,      EComponentType::Sint,       {false,  false, false,},             {1, 1}},
        { EResourceFormat::R8Uint,           "R8Uint",          1,              1,      EComponentType::Uint,       {false,  false, false,},             {1, 1}},
        { EResourceFormat::R16Int,           "R16Int",          2,              1,      EComponentType::Sint,       {false,  false, false,},             {1, 1}},
        { EResourceFormat::R16Uint,          "R16Uint",         2,              1,      EComponentType::Uint,       {false,  false, false,},             {1, 1}},
        { EResourceFormat::R32Int,           "R32Int",          4,              1,      EComponentType::Sint,       {false,  false, false,},             {1, 1}},
        { EResourceFormat::R32Uint,          "R32Uint",         4,              1,      EComponentType::Uint,       {false,  false, false,},             {1, 1}},
        { EResourceFormat::RG8Int,           "RG8Int",          2,              2,      EComponentType::Sint,       {false,  false, false,},             {1, 1}},
        { EResourceFormat::RG8Uint,          "RG8Uint",         2,              2,      EComponentType::Uint,       {false,  false, false,},             {1, 1}},
        { EResourceFormat::RG16Int,          "RG16Int",         4,              2,      EComponentType::Sint,       {false,  false, false,},             {1, 1}},
        { EResourceFormat::RG16Uint,         "RG16Uint",        4,              2,      EComponentType::Uint,       {false,  false, false,},             {1, 1}},
        { EResourceFormat::RG32Int,          "RG32Int",         8,              2,      EComponentType::Sint,       {false,  false, false,},             {1, 1}},
        { EResourceFormat::RG32Uint,         "RG32Uint",        8,              2,      EComponentType::Uint,       {false,  false, false,},             {1, 1}},
        // Format                           Name,           BytesPerBlock ChannelCount  Type                       {bDepth,   bStencil, bCompressed},    {CompressionRatio.Width,     CompressionRatio.Height}
        { EResourceFormat::RGB16Int,         "RGB16Int",        6,              3,      EComponentType::Sint,       {false,  false, false,},             {1, 1}},
        { EResourceFormat::RGB16Uint,        "RGB16Uint",       6,              3,      EComponentType::Uint,       {false,  false, false,},             {1, 1}},
        { EResourceFormat::RGB32Int,         "RGB32Int",       12,              3,      EComponentType::Sint,       {false,  false, false,},             {1, 1}},
        { EResourceFormat::RGB32Uint,        "RGB32Uint",      12,              3,      EComponentType::Uint,       {false,  false, false,},             {1, 1}},
        { EResourceFormat::RGBA8Int,         "RGBA8Int",        4,              4,      EComponentType::Sint,       {false,  false, false,},             {1, 1}},
        { EResourceFormat::RGBA8Uint,        "RGBA8Uint",       4,              4,      EComponentType::Uint,       {false, false, false, },             {1, 1}},
        { EResourceFormat::RGBA16Int,        "RGBA16Int",       8,              4,      EComponentType::Sint,       {false,  false, false,},             {1, 1}},
        { EResourceFormat::RGBA16Uint,       "RGBA16Uint",      8,              4,      EComponentType::Uint,       {false,  false, false,},             {1, 1}},
        { EResourceFormat::RGBA32Int,        "RGBA32Int",      16,              4,      EComponentType::Sint,       {false,  false, false,},             {1, 1}},
        { EResourceFormat::RGBA32Uint,       "RGBA32Uint",     16,              4,      EComponentType::Uint,       {false,  false, false,},             {1, 1}},
        { EResourceFormat::BGRA8Unorm,       "BGRA8Unorm",      4,              4,      EComponentType::Unorm,      {false,  false, false,},             {1, 1}},
        { EResourceFormat::BGRA8UnormSrgb,   "BGRA8UnormSrgb",  4,              4,      EComponentType::UnormSrgb,  {false,  false, false,},             {1, 1}},
        { EResourceFormat::BGRX8Unorm,       "BGRX8Unorm",      4,              4,      EComponentType::Unorm,      {false,  false, false,},             {1, 1}},
        { EResourceFormat::BGRX8UnormSrgb,   "BGRX8UnormSrgb",  4,              4,      EComponentType::UnormSrgb,  {false,  false, false,},             {1, 1}},
        { EResourceFormat::Alpha8Unorm,      "Alpha8Unorm",     1,              1,      EComponentType::Unorm,      {false,  false, false,},             {1, 1}},
        { EResourceFormat::Alpha32Float,     "Alpha32Float",    4,              1,      EComponentType::Float,      {false,  false, false,},             {1, 1}},
        // Format                           Name,           BytesPerBlock ChannelCount  Type                        {bDepth,   bStencil, bCompressed},   {CompressionRatio.Width,     CompressionRatio.Height}
        { EResourceFormat::R5G6B5Unorm,      "R5G6B5Unorm",     2,              3,      EComponentType::Unorm,      {false,  false, false,},             {1, 1}},
        { EResourceFormat::D32Float,         "D32Float",        4,              1,      EComponentType::Float,      {true,   false, false,},             {1, 1}},
        { EResourceFormat::D16Unorm,         "D16Unorm",        2,              1,      EComponentType::Unorm,      {true,   false, false,},             {1, 1}},
        { EResourceFormat::D32FloatS8X24,    "D32FloatS8X24",   8,              2,      EComponentType::Float,      {true,   true,  false,},             {1, 1}},
        { EResourceFormat::D24UnormS8,       "D24UnormS8",      4,              2,      EComponentType::Unorm,      {true,   true,  false,},             {1, 1}},
        { EResourceFormat::BC1Unorm,         "BC1Unorm",        8,              3,      EComponentType::Unorm,      {false,  false, true, },             {4, 4}},
        { EResourceFormat::BC1UnormSrgb,     "BC1UnormSrgb",    8,              3,      EComponentType::UnormSrgb,  {false,  false, true, },             {4, 4}},
        { EResourceFormat::BC2Unorm,         "BC2Unorm",        16,             4,      EComponentType::Unorm,      {false,  false, true, },             {4, 4}},
        { EResourceFormat::BC2UnormSrgb,     "BC2UnormSrgb",    16,             4,      EComponentType::UnormSrgb,  {false,  false, true, },             {4, 4}},
        { EResourceFormat::BC3Unorm,         "BC3Unorm",        16,             4,      EComponentType::Unorm,      {false,  false, true, },             {4, 4}},
        { EResourceFormat::BC3UnormSrgb,     "BC3UnormSrgb",    16,             4,      EComponentType::UnormSrgb,  {false,  false, true, },             {4, 4}},
        { EResourceFormat::BC4Unorm,         "BC4Unorm",        8,              1,      EComponentType::Unorm,      {false,  false, true, },             {4, 4}},
        { EResourceFormat::BC4Snorm,         "BC4Snorm",        8,              1,      EComponentType::Snorm,      {false,  false, true, },             {4, 4}},
        { EResourceFormat::BC5Unorm,         "BC5Unorm",        16,             2,      EComponentType::Unorm,      {false,  false, true, },             {4, 4}},
        { EResourceFormat::BC5Snorm,         "BC5Snorm",        16,             2,      EComponentType::Snorm,      {false,  false, true, },             {4, 4}},

        { EResourceFormat::BC6HS16,          "BC6HS16",         16,             3,      EComponentType::Float,      { false,  false, true, },            {4, 4}},
        { EResourceFormat::BC6HU16,          "BC6HU16",         16,             3,      EComponentType::Float,      { false,  false, true, },            {4, 4}},
        { EResourceFormat::BC7Unorm,         "BC7Unorm",        16,             4,      EComponentType::Unorm,      { false,  false, true, },            {4, 4}},
        { EResourceFormat::BC7UnormSrgb,     "BC7UnormSrgb",    16,             4,      EComponentType::UnormSrgb,  { false,  false, true, },            {4, 4}},
    };

    std::optional<ResourceFormatInfo> ResourceFormatInfo::Find(EResourceFormat format)
    {
        auto it = std::find_if(kResourceFormatInfoList.begin(), kResourceFormatInfoList.end(), 
                            [&](const ResourceFormatInfo& info) {
                                return info.format == format;        
                            });
        if (it == kResourceFormatInfoList.end())
            return std::nullopt;
        else
            return *it;    
    }

    std::optional<ResourceFormatInfo> ResourceFormatInfo::Find(const char* name)
    {
        auto it = std::find_if(kResourceFormatInfoList.begin(), kResourceFormatInfoList.end(), 
                            [&](const ResourceFormatInfo& info) {
                                return std::strcmp(info.name, name) == 0;        
                            });
        if (it == kResourceFormatInfoList.end())
            return std::nullopt;
        else
            return *it;  
    }

    EResourceFormat ResourceFormatInfo::SrgbToLinearFormat(EResourceFormat format)
    {
        switch (format) {
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
			return format;
		}
    }

    EResourceFormat ResourceFormatInfo::LinearToSrgbFormat(EResourceFormat format)
    {
        switch (format) {
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

    bool ResourceFormatInfo::HasAlpha(EResourceFormat format)
    {
        auto info = Find(format);
        if (!info.has_value())
            return false;

        if (info.value().channels == 4) {
            switch (format) {
            case EResourceFormat::BGRX8Unorm:
            case EResourceFormat::BGRX8UnormSrgb:
                return false;
            default:
                return true;
            }
        }

        switch (format) {
        case EResourceFormat::Alpha32Float:
        case EResourceFormat::Alpha8Unorm:
            return true;
        default:
            return false;
        }
    }
}