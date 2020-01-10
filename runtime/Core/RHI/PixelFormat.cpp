#include "Core/RHI/PixelFormat.h"
#include <unordered_map>

namespace Wanlix
{
    // Shortcuts for the format flags
    static const EPixelFormatFlags Depth = EPixelFormatFlags::HasDepth;
    static const EPixelFormatFlags Stencil = EPixelFormatFlags::HasStencil;
    static const EPixelFormatFlags sRGB = EPixelFormatFlags::IsColorSpace_sRGB;
    static const EPixelFormatFlags Compr = EPixelFormatFlags::IsCompressed;
    static const EPixelFormatFlags Norm = EPixelFormatFlags::IsNormalized;
    static const EPixelFormatFlags Integer = EPixelFormatFlags::IsInteger;
    static const EPixelFormatFlags Unsigned = EPixelFormatFlags::IsUnsigned;
    static const EPixelFormatFlags Packed = EPixelFormatFlags::IsPacked;
    static const EPixelFormatFlags RTV = EPixelFormatFlags::SupportsRenderTarget;
    static const EPixelFormatFlags Mips = EPixelFormatFlags::SupportsMips;
    static const EPixelFormatFlags GenMips = EPixelFormatFlags::SupportsGenerateMips | Mips | RTV;
    static const EPixelFormatFlags Dim1D = EPixelFormatFlags::SupportsTexture1D;
    static const EPixelFormatFlags Dim2D = EPixelFormatFlags::SupportsTexture2D;
    static const EPixelFormatFlags Dim3D = EPixelFormatFlags::SupportsTexture3D;
    static const EPixelFormatFlags DimCube = EPixelFormatFlags::SupportsTextureCube;
    static const EPixelFormatFlags Vertex = EPixelFormatFlags::SupportsVertex;

    static const EPixelFormatFlags Dim1D_2D = Dim1D | Dim2D;
    static const EPixelFormatFlags Dim2D_3D = Dim2D | Dim3D;
    static const EPixelFormatFlags Dim1D_2D_3D = Dim1D | Dim2D | Dim3D;
    static const EPixelFormatFlags UInt = Integer | Unsigned;
    static const EPixelFormatFlags SInt = Integer;
    static const EPixelFormatFlags UNorm = UInt | Norm;
    static const EPixelFormatFlags SNorm = SInt | Norm;
    static const EPixelFormatFlags SFloat = Unsigned;
    static const EPixelFormatFlags UFloat = EPixelFormatFlags::None;

    // Declaration of all hardware format descriptors
    static const PixelFormatDesc g_formatDescriptors[] =
    {
        //   bits  w  h  c 
        {   0, 0, 0, 0, EImageFormat::R,     EDataType::Undefined, EPixelFormatFlags::None                                    }, // Undefined

        /* --- Alpha channel color formats --- */
        //   bits  w  h  c  
        {   8, 1, 1, 1, EImageFormat::Alpha, EDataType::Uint8,     GenMips | Dim1D_2D_3D | DimCube | UNorm                    }, // A8UNorm

        /* --- Red channel color formats --- */
        //   bits  w  h  c  
        {   8, 1, 1, 1, EImageFormat::R,     EDataType::Uint8,     Vertex | GenMips | Dim1D_2D_3D | DimCube | UNorm           }, // R8UNorm
        {   8, 1, 1, 1, EImageFormat::R,     EDataType::Int8,      Vertex | GenMips | Dim1D_2D_3D | DimCube | UNorm           }, // R8SNorm
        {   8, 1, 1, 1, EImageFormat::R,     EDataType::Uint8,     Vertex | GenMips | Dim1D_2D_3D | DimCube | UInt            }, // R8UInt
        {   8, 1, 1, 1, EImageFormat::R,     EDataType::Int8,      Vertex | GenMips | Dim1D_2D_3D | DimCube | SInt            }, // R8SInt

        {  16, 1, 1, 1, EImageFormat::R,     EDataType::Uint16,    Vertex | GenMips | Dim1D_2D_3D | DimCube | UNorm           }, // R16UNorm
        {  16, 1, 1, 1, EImageFormat::R,     EDataType::Int16,     Vertex | GenMips | Dim1D_2D_3D | DimCube | SNorm           }, // R16SNorm
        {  16, 1, 1, 1, EImageFormat::R,     EDataType::Uint16,    Vertex | GenMips | Dim1D_2D_3D | DimCube | UInt            }, // R16UInt
        {  16, 1, 1, 1, EImageFormat::R,     EDataType::Int16,     Vertex | GenMips | Dim1D_2D_3D | DimCube | SInt            }, // R16SInt
        {  16, 1, 1, 1, EImageFormat::R,     EDataType::Float16,   Vertex | GenMips | Dim1D_2D_3D | DimCube | SFloat          }, // R16Float

        {  32, 1, 1, 1, EImageFormat::R,     EDataType::Uint32,    Vertex | GenMips | Dim1D_2D_3D | DimCube | UInt            }, // R32UInt
        {  32, 1, 1, 1, EImageFormat::R,     EDataType::Int32,     Vertex | GenMips | Dim1D_2D_3D | DimCube | SInt            }, // R32SInt
        {  32, 1, 1, 1, EImageFormat::R,     EDataType::Float32,   Vertex | GenMips | Dim1D_2D_3D | DimCube | SFloat          }, // R32Float

        {  64, 1, 1, 1, EImageFormat::R,     EDataType::Float64,   Vertex | GenMips | Dim1D_2D_3D | DimCube | SFloat          }, // R64Float

        /* --- RG color formats --- */
        //   bits  w  h  c  
        {  16, 1, 1, 2, EImageFormat::RG,    EDataType::Uint8,     Vertex | GenMips | Dim1D_2D_3D | DimCube | UNorm           }, // RG8UNorm
        {  16, 1, 1, 2, EImageFormat::RG,    EDataType::Int8,      Vertex | GenMips | Dim1D_2D_3D | DimCube | SNorm           }, // RG8SNorm
        {  16, 1, 1, 2, EImageFormat::RG,    EDataType::Uint8,     Vertex | GenMips | Dim1D_2D_3D | DimCube | UInt            }, // RG8UInt
        {  16, 1, 1, 2, EImageFormat::RG,    EDataType::Int8,      Vertex | GenMips | Dim1D_2D_3D | DimCube | SInt            }, // RG8SInt

        {  32, 1, 1, 2, EImageFormat::RG,    EDataType::Uint16,    Vertex | GenMips | Dim1D_2D_3D | DimCube | UNorm           }, // RG16UNorm
        {  32, 1, 1, 2, EImageFormat::RG,    EDataType::Int16,     Vertex | GenMips | Dim1D_2D_3D | DimCube | SNorm           }, // RG16SNorm
        {  32, 1, 1, 2, EImageFormat::RG,    EDataType::Uint16,    Vertex | GenMips | Dim1D_2D_3D | DimCube | UInt            }, // RG16UInt
        {  32, 1, 1, 2, EImageFormat::RG,    EDataType::Int16,     Vertex | GenMips | Dim1D_2D_3D | DimCube | SInt            }, // RG16SInt
        {  32, 1, 1, 2, EImageFormat::RG,    EDataType::Float16,   Vertex | GenMips | Dim1D_2D_3D | DimCube | SFloat          }, // RG16Float

        {  64, 1, 1, 2, EImageFormat::RG,    EDataType::Uint32,    Vertex | GenMips | Dim1D_2D_3D | DimCube | UInt            }, // RG32UInt
        {  64, 1, 1, 2, EImageFormat::RG,    EDataType::Int32,     Vertex | GenMips | Dim1D_2D_3D | DimCube | SInt            }, // RG32SInt
        {  64, 1, 1, 2, EImageFormat::RG,    EDataType::Float32,   Vertex | GenMips | Dim1D_2D_3D | DimCube | SFloat          }, // RG32Float

        { 128, 1, 1, 2, EImageFormat::RG,    EDataType::Float64,   Vertex | GenMips | Dim1D_2D_3D | DimCube | SFloat          }, // RG64Float

        /* --- RGB color formats --- */
        //   bits  w  h  c  
        {  24, 1, 1, 3, EImageFormat::RGB,   EDataType::Uint8,     Vertex | GenMips | Dim1D_2D_3D | DimCube | UNorm           }, // RGB8UNorm
        {  24, 1, 1, 3, EImageFormat::RGB,   EDataType::Uint8,     Vertex | GenMips | Dim1D_2D_3D | DimCube | UNorm | sRGB    }, // RGB8UNorm_sRGB
        {  24, 1, 1, 3, EImageFormat::RGB,   EDataType::Int8,      Vertex | GenMips | Dim1D_2D_3D | DimCube | SNorm           }, // RGB8SNorm
        {  24, 1, 1, 3, EImageFormat::RGB,   EDataType::Uint8,     Vertex | GenMips | Dim1D_2D_3D | DimCube | UInt            }, // RGB8UInt
        {  24, 1, 1, 3, EImageFormat::RGB,   EDataType::Int8,      Vertex | GenMips | Dim1D_2D_3D | DimCube | SInt            }, // RGB8SInt

        {  48, 1, 1, 3, EImageFormat::RGB,   EDataType::Uint16,    Vertex | GenMips | Dim1D_2D_3D | DimCube | UNorm           }, // RGB16UNorm
        {  48, 1, 1, 3, EImageFormat::RGB,   EDataType::Int16,     Vertex | GenMips | Dim1D_2D_3D | DimCube | SNorm           }, // RGB16SNorm
        {  48, 1, 1, 3, EImageFormat::RGB,   EDataType::Uint16,    Vertex | GenMips | Dim1D_2D_3D | DimCube | UInt            }, // RGB16UInt
        {  48, 1, 1, 3, EImageFormat::RGB,   EDataType::Int16,     Vertex | GenMips | Dim1D_2D_3D | DimCube | SInt            }, // RGB16SInt
        {  48, 1, 1, 3, EImageFormat::RGB,   EDataType::Float16,   Vertex | GenMips | Dim1D_2D_3D | DimCube | SFloat          }, // RGB16Float

        {  96, 1, 1, 3, EImageFormat::RGB,   EDataType::Uint32,    Vertex | GenMips | Dim1D_2D_3D | DimCube | UInt            }, // RGB32UInt
        {  96, 1, 1, 3, EImageFormat::RGB,   EDataType::Int32,     Vertex | GenMips | Dim1D_2D_3D | DimCube | SInt            }, // RGB32SInt
        {  96, 1, 1, 3, EImageFormat::RGB,   EDataType::Float32,   Vertex | GenMips | Dim1D_2D_3D | DimCube | SFloat          }, // RGB32Float

        { 192, 1, 1, 3, EImageFormat::RGB,   EDataType::Float64,   Vertex | GenMips | Dim1D_2D_3D | DimCube | SFloat          }, // RGB64Float

        /* --- RGBA color formats --- */
        //   bits  w  h  c  
        {  32, 1, 1, 4, EImageFormat::RGBA,  EDataType::Uint8,     Vertex | GenMips | Dim1D_2D_3D | DimCube | UNorm           }, // RGBA8UNorm
        {  32, 1, 1, 4, EImageFormat::RGBA,  EDataType::Uint8,     Vertex | GenMips | Dim1D_2D_3D | DimCube | UNorm | sRGB    }, // RGBA8UNorm_sRGB
        {  32, 1, 1, 4, EImageFormat::RGBA,  EDataType::Int8,      Vertex | GenMips | Dim1D_2D_3D | DimCube | SNorm           }, // RGBA8SNorm
        {  32, 1, 1, 4, EImageFormat::RGBA,  EDataType::Uint8,     Vertex | GenMips | Dim1D_2D_3D | DimCube | UInt            }, // RGBA8UInt
        {  32, 1, 1, 4, EImageFormat::RGBA,  EDataType::Int8,      Vertex | GenMips | Dim1D_2D_3D | DimCube | SInt            }, // RGBA8SInt

        {  64, 1, 1, 4, EImageFormat::RGBA,  EDataType::Uint16,    Vertex | GenMips | Dim1D_2D_3D | DimCube | UNorm           }, // RGBA16UNorm
        {  64, 1, 1, 4, EImageFormat::RGBA,  EDataType::Int16,     Vertex | GenMips | Dim1D_2D_3D | DimCube | SNorm           }, // RGBA16SNorm
        {  64, 1, 1, 4, EImageFormat::RGBA,  EDataType::Uint16,    Vertex | GenMips | Dim1D_2D_3D | DimCube | UInt            }, // RGBA16UInt
        {  64, 1, 1, 4, EImageFormat::RGBA,  EDataType::Int16,     Vertex | GenMips | Dim1D_2D_3D | DimCube | SInt            }, // RGBA16SInt
        {  64, 1, 1, 4, EImageFormat::RGBA,  EDataType::Float16,   Vertex | GenMips | Dim1D_2D_3D | DimCube | SFloat          }, // RGBA16Float

        { 128, 1, 1, 4, EImageFormat::RGBA,  EDataType::Uint32,    Vertex | GenMips | Dim1D_2D_3D | DimCube | UInt            }, // RGBA32UInt
        { 128, 1, 1, 4, EImageFormat::RGBA,  EDataType::Int32,     Vertex | GenMips | Dim1D_2D_3D | DimCube | SInt            }, // RGBA32SInt
        { 128, 1, 1, 4, EImageFormat::RGBA,  EDataType::Float32,   Vertex | GenMips | Dim1D_2D_3D | DimCube | SFloat          }, // RGBA32Float

        { 256, 1, 1, 4, EImageFormat::RGBA,  EDataType::Float64,   Vertex | GenMips | Dim1D_2D_3D | DimCube | SFloat          }, // RGBA64Float

        /* --- BGRA color formats --- */
        //   bits  w  h  c  
        {  32, 1, 1, 4, EImageFormat::BGRA,  EDataType::Uint8,     GenMips | Dim1D_2D_3D | DimCube | UNorm                    }, // BGRA8UNorm
        {  32, 1, 1, 4, EImageFormat::BGRA,  EDataType::Uint8,     GenMips | Dim1D_2D_3D | DimCube | UNorm | sRGB             }, // BGRA8UNorm_sRGB
        {  32, 1, 1, 4, EImageFormat::BGRA,  EDataType::Int8,      GenMips | Dim1D_2D_3D | DimCube | SNorm                    }, // BGRA8SNorm
        {  32, 1, 1, 4, EImageFormat::BGRA,  EDataType::Uint8,     GenMips | Dim1D_2D_3D | DimCube | UInt                     }, // BGRA8UInt
        {  32, 1, 1, 4, EImageFormat::BGRA,  EDataType::Int8,      GenMips | Dim1D_2D_3D | DimCube | SInt                     }, // BGRA8SInt

        /* --- Packed formats --- */
        //   bits  w  h  c  
        {  32, 1, 1, 4, EImageFormat::RGBA,  EDataType::Undefined, GenMips | Dim1D_2D_3D | DimCube | UNorm  | Packed          }, // RGB10A2UNorm
        {  32, 1, 1, 4, EImageFormat::RGBA,  EDataType::Undefined, GenMips | Dim1D_2D_3D | DimCube | UInt   | Packed          }, // RGB10A2UInt
        {  32, 1, 1, 3, EImageFormat::RGB,   EDataType::Undefined, GenMips | Dim1D_2D_3D | DimCube | UFloat | Packed          }, // RG11B10Float
        {  32, 1, 1, 3, EImageFormat::RGB,   EDataType::Undefined, Mips | Dim1D_2D_3D | DimCube | UFloat    | Packed          }, // RGB9E5Float

        /* --- Depth-stencil formats --- */
        //   bits  w  h  c  
        {  16, 1, 1, 1, EImageFormat::Depth,        EDataType::Uint16,    Mips | RTV | Dim1D_2D | DimCube | UNorm  | Depth           }, // D16UNorm
        {  32, 1, 1, 2, EImageFormat::DepthStencil, EDataType::Uint16,    Mips | RTV | Dim1D_2D | DimCube | UNorm  | Depth | Stencil }, // D24UNormS8UInt
        {  32, 1, 1, 1, EImageFormat::Depth,        EDataType::Float32,   Mips | RTV | Dim1D_2D | DimCube | SFloat | Depth           }, // D32Float
        {  64, 1, 1, 2, EImageFormat::DepthStencil, EDataType::Float32,   Mips | RTV | Dim1D_2D | DimCube | SFloat | Depth | Stencil }, // D32FloatS8X24UInt
        {   8, 1, 1, 1, EImageFormat::Stencil,      EDataType::Uint8,     Mips | RTV | Dim1D_2D | DimCube | UInt   | Stencil         }, // S8UInt

        /* --- Block compression (BC) formats --- */
        //   bits  w  h  c  
        {  64, 4, 4, 4, EImageFormat::BC1,    EDataType::Uint8,     Mips | Dim2D_3D | DimCube | Compr | UNorm                  }, // BC1UNorm
        {  64, 4, 4, 4, EImageFormat::BC1,    EDataType::Uint8,     Mips | Dim2D_3D | DimCube | Compr | UNorm | sRGB           }, // BC1UNorm_sRGB
        { 128, 4, 4, 4, EImageFormat::BC2,    EDataType::Uint8,     Mips | Dim2D_3D | DimCube | Compr | UNorm                  }, // BC2UNorm
        { 128, 4, 4, 4, EImageFormat::BC2,    EDataType::Uint8,     Mips | Dim2D_3D | DimCube | Compr | UNorm | sRGB           }, // BC2UNorm_sRGB
        { 128, 4, 4, 4, EImageFormat::BC3,    EDataType::Uint8,     Mips | Dim2D_3D | DimCube | Compr | UNorm                  }, // BC3UNorm
        { 128, 4, 4, 4, EImageFormat::BC3,    EDataType::Uint8,     Mips | Dim2D_3D | DimCube | Compr | UNorm | sRGB           }, // BC3UNorm_sRGB
        {  64, 4, 4, 1, EImageFormat::BC4,    EDataType::Uint8,     Mips | Dim2D_3D | DimCube | Compr | UNorm                  }, // BC4UNorm
        {  64, 4, 4, 1, EImageFormat::BC4,    EDataType::Int8,      Mips | Dim2D_3D | DimCube | Compr | SNorm                  }, // BC4SNorm
        { 128, 4, 4, 2, EImageFormat::BC5,    EDataType::Uint8,     Mips | Dim2D_3D | DimCube | Compr | UNorm                  }, // BC5UNorm
        { 128, 4, 4, 2, EImageFormat::BC5,    EDataType::Int8,      Mips | Dim2D_3D | DimCube | Compr | SNorm                  }, // BC5SNorm
    };

    const PixelFormatDesc& GetPixelFormatDesc(EPixelFormat format)
    {
        static const uint32_t kLength = sizeof(g_formatDescriptors) / sizeof(g_formatDescriptors[0]);
        
        auto idx = static_cast<uint32_t>(format);
        if (idx < kLength) {
            return g_formatDescriptors[idx];
        }
        else {
            return g_formatDescriptors[0];
        }
    }

    const PixelFormatDesc& GetPixelFormatDesc(EImageFormat imageFormat, EDataType dataType)
    {
        static const uint32_t kLength = sizeof(g_formatDescriptors) / sizeof(g_formatDescriptors[0]);

        uint32_t i = 0;
        for (; i < kLength; i++) {
            const PixelFormatDesc& desc = g_formatDescriptors[i];
            if (desc.imageFormat == imageFormat && desc.dataType == dataType) {
                break;
            }
        }
        if (i == kLength) {
            i = 0;
        }

        return g_formatDescriptors[i];
    }

    bool PixelFormatDesc::IsCompressedFormat() const
    {
        return HasBits(flags, EPixelFormatFlags::IsCompressed);
    }

    bool PixelFormatDesc::IsDepthStencilFormat() const
    {
        return HasBits(flags, EPixelFormatFlags::HasDepth | EPixelFormatFlags::HasStencil);
    }

    bool PixelFormatDesc::IsDepthFormat() const
    {
        return HasBits(flags, EPixelFormatFlags::HasDepth);
    }

    bool PixelFormatDesc::IsStencilFormat() const
    {
        return HasBits(flags, EPixelFormatFlags::HasStencil);
    }

    bool PixelFormatDesc::IsNormalizedFormat() const
    {
        return HasBits(flags, EPixelFormatFlags::IsNormalized);
    }

    bool PixelFormatDesc::IsIntegralFormat() const
    {
        return HasBits(flags, EPixelFormatFlags::IsInteger);
    }

    bool PixelFormatDesc::IsFloatFormat() const
    {
        return dataType == EDataType::Float16 ||
               dataType == EDataType::Float32 ||
               dataType == EDataType::Float64;
    }

    uint32_t PixelFormatDesc::GetSizeInBytes() const
    {
        return bits / 8;
    }
}