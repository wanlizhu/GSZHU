#include "Wanlix/RHI/Format.h"
#include <unordered_map>

namespace Wanlix
{
    // Shortcuts for the format flags
    static const uint32_t Depth = PixelFormatFlags::HasDepth;
    static const uint32_t Stencil = PixelFormatFlags::HasStencil;
    static const uint32_t sRGB = PixelFormatFlags::IsColorSpace_sRGB;
    static const uint32_t Compr = PixelFormatFlags::IsCompressed;
    static const uint32_t Norm = PixelFormatFlags::IsNormalized;
    static const uint32_t Integer = PixelFormatFlags::IsInteger;
    static const uint32_t Unsigned = PixelFormatFlags::IsUnsigned;
    static const uint32_t Packed = PixelFormatFlags::IsPacked;
    static const uint32_t RTV = PixelFormatFlags::SupportsRenderTarget;
    static const uint32_t Mips = PixelFormatFlags::SupportsMips;
    static const uint32_t GenMips = PixelFormatFlags::SupportsGenerateMips | Mips | RTV;
    static const uint32_t Dim1D = PixelFormatFlags::SupportsTexture1D;
    static const uint32_t Dim2D = PixelFormatFlags::SupportsTexture2D;
    static const uint32_t Dim3D = PixelFormatFlags::SupportsTexture3D;
    static const uint32_t DimCube = PixelFormatFlags::SupportsTextureCube;
    static const uint32_t Vertex = PixelFormatFlags::SupportsVertex;

    static const uint32_t Dim1D_2D = Dim1D | Dim2D;
    static const uint32_t Dim2D_3D = Dim2D | Dim3D;
    static const uint32_t Dim1D_2D_3D = Dim1D | Dim2D | Dim3D;
    static const uint32_t UInt = Integer | Unsigned;
    static const uint32_t SInt = Integer;
    static const uint32_t UNorm = UInt | Norm;
    static const uint32_t SNorm = SInt | Norm;
    static const uint32_t SFloat = Unsigned;
    static const uint32_t UFloat = 0;

    // Declaration of all hardware format descriptors
    static const FormatDescriptor g_formatDescriptors[] =
    {
        //   bits  w  h  c 
        {   0, 0, 0, 0, ImageFormat::R,     DataType::Undefined, 0                                                          }, // Undefined

        /* --- Alpha channel color formats --- */
        //   bits  w  h  c  
        {   8, 1, 1, 1, ImageFormat::Alpha, DataType::Uint8,     GenMips | Dim1D_2D_3D | DimCube | UNorm                    }, // A8UNorm

        /* --- Red channel color formats --- */
        //   bits  w  h  c  
        {   8, 1, 1, 1, ImageFormat::R,     DataType::Uint8,     Vertex | GenMips | Dim1D_2D_3D | DimCube | UNorm           }, // R8UNorm
        {   8, 1, 1, 1, ImageFormat::R,     DataType::Int8,      Vertex | GenMips | Dim1D_2D_3D | DimCube | UNorm           }, // R8SNorm
        {   8, 1, 1, 1, ImageFormat::R,     DataType::Uint8,     Vertex | GenMips | Dim1D_2D_3D | DimCube | UInt            }, // R8UInt
        {   8, 1, 1, 1, ImageFormat::R,     DataType::Int8,      Vertex | GenMips | Dim1D_2D_3D | DimCube | SInt            }, // R8SInt

        {  16, 1, 1, 1, ImageFormat::R,     DataType::Uint16,    Vertex | GenMips | Dim1D_2D_3D | DimCube | UNorm           }, // R16UNorm
        {  16, 1, 1, 1, ImageFormat::R,     DataType::Int16,     Vertex | GenMips | Dim1D_2D_3D | DimCube | SNorm           }, // R16SNorm
        {  16, 1, 1, 1, ImageFormat::R,     DataType::Uint16,    Vertex | GenMips | Dim1D_2D_3D | DimCube | UInt            }, // R16UInt
        {  16, 1, 1, 1, ImageFormat::R,     DataType::Int16,     Vertex | GenMips | Dim1D_2D_3D | DimCube | SInt            }, // R16SInt
        {  16, 1, 1, 1, ImageFormat::R,     DataType::Float16,   Vertex | GenMips | Dim1D_2D_3D | DimCube | SFloat          }, // R16Float

        {  32, 1, 1, 1, ImageFormat::R,     DataType::Uint32,    Vertex | GenMips | Dim1D_2D_3D | DimCube | UInt            }, // R32UInt
        {  32, 1, 1, 1, ImageFormat::R,     DataType::Int32,     Vertex | GenMips | Dim1D_2D_3D | DimCube | SInt            }, // R32SInt
        {  32, 1, 1, 1, ImageFormat::R,     DataType::Float32,   Vertex | GenMips | Dim1D_2D_3D | DimCube | SFloat          }, // R32Float

        {  64, 1, 1, 1, ImageFormat::R,     DataType::Float64,   Vertex | GenMips | Dim1D_2D_3D | DimCube | SFloat          }, // R64Float

        /* --- RG color formats --- */
        //   bits  w  h  c  
        {  16, 1, 1, 2, ImageFormat::RG,    DataType::Uint8,     Vertex | GenMips | Dim1D_2D_3D | DimCube | UNorm           }, // RG8UNorm
        {  16, 1, 1, 2, ImageFormat::RG,    DataType::Int8,      Vertex | GenMips | Dim1D_2D_3D | DimCube | SNorm           }, // RG8SNorm
        {  16, 1, 1, 2, ImageFormat::RG,    DataType::Uint8,     Vertex | GenMips | Dim1D_2D_3D | DimCube | UInt            }, // RG8UInt
        {  16, 1, 1, 2, ImageFormat::RG,    DataType::Int8,      Vertex | GenMips | Dim1D_2D_3D | DimCube | SInt            }, // RG8SInt

        {  32, 1, 1, 2, ImageFormat::RG,    DataType::Uint16,    Vertex | GenMips | Dim1D_2D_3D | DimCube | UNorm           }, // RG16UNorm
        {  32, 1, 1, 2, ImageFormat::RG,    DataType::Int16,     Vertex | GenMips | Dim1D_2D_3D | DimCube | SNorm           }, // RG16SNorm
        {  32, 1, 1, 2, ImageFormat::RG,    DataType::Uint16,    Vertex | GenMips | Dim1D_2D_3D | DimCube | UInt            }, // RG16UInt
        {  32, 1, 1, 2, ImageFormat::RG,    DataType::Int16,     Vertex | GenMips | Dim1D_2D_3D | DimCube | SInt            }, // RG16SInt
        {  32, 1, 1, 2, ImageFormat::RG,    DataType::Float16,   Vertex | GenMips | Dim1D_2D_3D | DimCube | SFloat          }, // RG16Float

        {  64, 1, 1, 2, ImageFormat::RG,    DataType::Uint32,    Vertex | GenMips | Dim1D_2D_3D | DimCube | UInt            }, // RG32UInt
        {  64, 1, 1, 2, ImageFormat::RG,    DataType::Int32,     Vertex | GenMips | Dim1D_2D_3D | DimCube | SInt            }, // RG32SInt
        {  64, 1, 1, 2, ImageFormat::RG,    DataType::Float32,   Vertex | GenMips | Dim1D_2D_3D | DimCube | SFloat          }, // RG32Float

        { 128, 1, 1, 2, ImageFormat::RG,    DataType::Float64,   Vertex | GenMips | Dim1D_2D_3D | DimCube | SFloat          }, // RG64Float

        /* --- RGB color formats --- */
        //   bits  w  h  c  
        {  24, 1, 1, 3, ImageFormat::RGB,   DataType::Uint8,     Vertex | GenMips | Dim1D_2D_3D | DimCube | UNorm           }, // RGB8UNorm
        {  24, 1, 1, 3, ImageFormat::RGB,   DataType::Uint8,     Vertex | GenMips | Dim1D_2D_3D | DimCube | UNorm | sRGB    }, // RGB8UNorm_sRGB
        {  24, 1, 1, 3, ImageFormat::RGB,   DataType::Int8,      Vertex | GenMips | Dim1D_2D_3D | DimCube | SNorm           }, // RGB8SNorm
        {  24, 1, 1, 3, ImageFormat::RGB,   DataType::Uint8,     Vertex | GenMips | Dim1D_2D_3D | DimCube | UInt            }, // RGB8UInt
        {  24, 1, 1, 3, ImageFormat::RGB,   DataType::Int8,      Vertex | GenMips | Dim1D_2D_3D | DimCube | SInt            }, // RGB8SInt

        {  48, 1, 1, 3, ImageFormat::RGB,   DataType::Uint16,    Vertex | GenMips | Dim1D_2D_3D | DimCube | UNorm           }, // RGB16UNorm
        {  48, 1, 1, 3, ImageFormat::RGB,   DataType::Int16,     Vertex | GenMips | Dim1D_2D_3D | DimCube | SNorm           }, // RGB16SNorm
        {  48, 1, 1, 3, ImageFormat::RGB,   DataType::Uint16,    Vertex | GenMips | Dim1D_2D_3D | DimCube | UInt            }, // RGB16UInt
        {  48, 1, 1, 3, ImageFormat::RGB,   DataType::Int16,     Vertex | GenMips | Dim1D_2D_3D | DimCube | SInt            }, // RGB16SInt
        {  48, 1, 1, 3, ImageFormat::RGB,   DataType::Float16,   Vertex | GenMips | Dim1D_2D_3D | DimCube | SFloat          }, // RGB16Float

        {  96, 1, 1, 3, ImageFormat::RGB,   DataType::Uint32,    Vertex | GenMips | Dim1D_2D_3D | DimCube | UInt            }, // RGB32UInt
        {  96, 1, 1, 3, ImageFormat::RGB,   DataType::Int32,     Vertex | GenMips | Dim1D_2D_3D | DimCube | SInt            }, // RGB32SInt
        {  96, 1, 1, 3, ImageFormat::RGB,   DataType::Float32,   Vertex | GenMips | Dim1D_2D_3D | DimCube | SFloat          }, // RGB32Float

        { 192, 1, 1, 3, ImageFormat::RGB,   DataType::Float64,   Vertex | GenMips | Dim1D_2D_3D | DimCube | SFloat          }, // RGB64Float

        /* --- RGBA color formats --- */
        //   bits  w  h  c  
        {  32, 1, 1, 4, ImageFormat::RGBA,  DataType::Uint8,     Vertex | GenMips | Dim1D_2D_3D | DimCube | UNorm           }, // RGBA8UNorm
        {  32, 1, 1, 4, ImageFormat::RGBA,  DataType::Uint8,     Vertex | GenMips | Dim1D_2D_3D | DimCube | UNorm | sRGB    }, // RGBA8UNorm_sRGB
        {  32, 1, 1, 4, ImageFormat::RGBA,  DataType::Int8,      Vertex | GenMips | Dim1D_2D_3D | DimCube | SNorm           }, // RGBA8SNorm
        {  32, 1, 1, 4, ImageFormat::RGBA,  DataType::Uint8,     Vertex | GenMips | Dim1D_2D_3D | DimCube | UInt            }, // RGBA8UInt
        {  32, 1, 1, 4, ImageFormat::RGBA,  DataType::Int8,      Vertex | GenMips | Dim1D_2D_3D | DimCube | SInt            }, // RGBA8SInt

        {  64, 1, 1, 4, ImageFormat::RGBA,  DataType::Uint16,    Vertex | GenMips | Dim1D_2D_3D | DimCube | UNorm           }, // RGBA16UNorm
        {  64, 1, 1, 4, ImageFormat::RGBA,  DataType::Int16,     Vertex | GenMips | Dim1D_2D_3D | DimCube | SNorm           }, // RGBA16SNorm
        {  64, 1, 1, 4, ImageFormat::RGBA,  DataType::Uint16,    Vertex | GenMips | Dim1D_2D_3D | DimCube | UInt            }, // RGBA16UInt
        {  64, 1, 1, 4, ImageFormat::RGBA,  DataType::Int16,     Vertex | GenMips | Dim1D_2D_3D | DimCube | SInt            }, // RGBA16SInt
        {  64, 1, 1, 4, ImageFormat::RGBA,  DataType::Float16,   Vertex | GenMips | Dim1D_2D_3D | DimCube | SFloat          }, // RGBA16Float

        { 128, 1, 1, 4, ImageFormat::RGBA,  DataType::Uint32,    Vertex | GenMips | Dim1D_2D_3D | DimCube | UInt            }, // RGBA32UInt
        { 128, 1, 1, 4, ImageFormat::RGBA,  DataType::Int32,     Vertex | GenMips | Dim1D_2D_3D | DimCube | SInt            }, // RGBA32SInt
        { 128, 1, 1, 4, ImageFormat::RGBA,  DataType::Float32,   Vertex | GenMips | Dim1D_2D_3D | DimCube | SFloat          }, // RGBA32Float

        { 256, 1, 1, 4, ImageFormat::RGBA,  DataType::Float64,   Vertex | GenMips | Dim1D_2D_3D | DimCube | SFloat          }, // RGBA64Float

        /* --- BGRA color formats --- */
        //   bits  w  h  c  
        {  32, 1, 1, 4, ImageFormat::BGRA,  DataType::Uint8,     GenMips | Dim1D_2D_3D | DimCube | UNorm                    }, // BGRA8UNorm
        {  32, 1, 1, 4, ImageFormat::BGRA,  DataType::Uint8,     GenMips | Dim1D_2D_3D | DimCube | UNorm | sRGB             }, // BGRA8UNorm_sRGB
        {  32, 1, 1, 4, ImageFormat::BGRA,  DataType::Int8,      GenMips | Dim1D_2D_3D | DimCube | SNorm                    }, // BGRA8SNorm
        {  32, 1, 1, 4, ImageFormat::BGRA,  DataType::Uint8,     GenMips | Dim1D_2D_3D | DimCube | UInt                     }, // BGRA8UInt
        {  32, 1, 1, 4, ImageFormat::BGRA,  DataType::Int8,      GenMips | Dim1D_2D_3D | DimCube | SInt                     }, // BGRA8SInt

        /* --- Packed formats --- */
        //   bits  w  h  c  
        {  32, 1, 1, 4, ImageFormat::RGBA,  DataType::Undefined, GenMips | Dim1D_2D_3D | DimCube | UNorm | Packed          }, // RGB10A2UNorm
        {  32, 1, 1, 4, ImageFormat::RGBA,  DataType::Undefined, GenMips | Dim1D_2D_3D | DimCube | UInt | Packed          }, // RGB10A2UInt
        {  32, 1, 1, 3, ImageFormat::RGB,   DataType::Undefined, GenMips | Dim1D_2D_3D | DimCube | UFloat | Packed          }, // RG11B10Float
        {  32, 1, 1, 3, ImageFormat::RGB,   DataType::Undefined, Mips | Dim1D_2D_3D | DimCube | UFloat | Packed          }, // RGB9E5Float

        /* --- Depth-stencil formats --- */
        //   bits  w  h  c  
        {  16, 1, 1, 1, ImageFormat::Depth,        DataType::Uint16,    Mips | RTV | Dim1D_2D | DimCube | UNorm | Depth           }, // D16UNorm
        {  32, 1, 1, 2, ImageFormat::DepthStencil, DataType::Uint16,    Mips | RTV | Dim1D_2D | DimCube | UNorm | Depth | Stencil }, // D24UNormS8UInt
        {  32, 1, 1, 1, ImageFormat::Depth,        DataType::Float32,   Mips | RTV | Dim1D_2D | DimCube | SFloat | Depth           }, // D32Float
        {  64, 1, 1, 2, ImageFormat::DepthStencil, DataType::Float32,   Mips | RTV | Dim1D_2D | DimCube | SFloat | Depth | Stencil }, // D32FloatS8X24UInt
        {   8, 1, 1, 1, ImageFormat::Stencil,      DataType::Uint8,     Mips | RTV | Dim1D_2D | DimCube | UInt   | Stencil         }, // S8UInt

        /* --- Block compression (BC) formats --- */
        //   bits  w  h  c  
        {  64, 4, 4, 4, ImageFormat::BC1,    DataType::Uint8,     Mips | Dim2D_3D | DimCube | Compr | UNorm                  }, // BC1UNorm
        {  64, 4, 4, 4, ImageFormat::BC1,    DataType::Uint8,     Mips | Dim2D_3D | DimCube | Compr | UNorm | sRGB           }, // BC1UNorm_sRGB
        { 128, 4, 4, 4, ImageFormat::BC2,    DataType::Uint8,     Mips | Dim2D_3D | DimCube | Compr | UNorm                  }, // BC2UNorm
        { 128, 4, 4, 4, ImageFormat::BC2,    DataType::Uint8,     Mips | Dim2D_3D | DimCube | Compr | UNorm | sRGB           }, // BC2UNorm_sRGB
        { 128, 4, 4, 4, ImageFormat::BC3,    DataType::Uint8,     Mips | Dim2D_3D | DimCube | Compr | UNorm                  }, // BC3UNorm
        { 128, 4, 4, 4, ImageFormat::BC3,    DataType::Uint8,     Mips | Dim2D_3D | DimCube | Compr | UNorm | sRGB           }, // BC3UNorm_sRGB
        {  64, 4, 4, 1, ImageFormat::BC4,    DataType::Uint8,     Mips | Dim2D_3D | DimCube | Compr | UNorm                  }, // BC4UNorm
        {  64, 4, 4, 1, ImageFormat::BC4,    DataType::Int8,      Mips | Dim2D_3D | DimCube | Compr | SNorm                  }, // BC4SNorm
        { 128, 4, 4, 2, ImageFormat::BC5,    DataType::Uint8,     Mips | Dim2D_3D | DimCube | Compr | UNorm                  }, // BC5UNorm
        { 128, 4, 4, 2, ImageFormat::BC5,    DataType::Int8,      Mips | Dim2D_3D | DimCube | Compr | SNorm                  }, // BC5SNorm
    };

    const FormatDescriptor& GetFormatDescritpor(PixelFormat format)
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

    const FormatDescriptor& GetFormatDescritpor(ImageFormat imageFormat, DataType dataType)
    {
        static const uint32_t kLength = sizeof(g_formatDescriptors) / sizeof(g_formatDescriptors[0]);

        uint32_t i = 0;
        for (; i < kLength; i++) {
            const FormatDescriptor& desc = g_formatDescriptors[i];
            if (desc.imageFormat == imageFormat && desc.dataType == dataType) {
                break;
            }
        }
        if (i == kLength) {
            i = 0;
        }

        return g_formatDescriptors[i];
    }

    bool FormatDescriptor::IsCompressedFormat() const
    {
        return (flags & PixelFormatFlags::IsCompressed) != 0;
    }

    bool FormatDescriptor::IsDepthStencilFormat() const
    {
        return (flags & (PixelFormatFlags::HasDepth | PixelFormatFlags::HasStencil)) != 0;
    }

    bool FormatDescriptor::IsDepthFormat() const
    {
        return (flags & PixelFormatFlags::HasDepth) != 0;
    }

    bool FormatDescriptor::IsStencilFormat() const
    {
        return (flags & PixelFormatFlags::HasStencil) != 0;
    }

    bool FormatDescriptor::IsNormalizedFormat() const
    {
        return (flags & PixelFormatFlags::IsNormalized) != 0;
    }

    bool FormatDescriptor::IsIntegralFormat() const
    {
        return (flags & PixelFormatFlags::IsInteger) != 0;
    }

    bool FormatDescriptor::IsFloatFormat() const
    {
        return dataType == DataType::Float16 ||
               dataType == DataType::Float32 ||
               dataType == DataType::Float64;
    }

    uint32_t FormatDescriptor::GetSize() const
    {
        return bits / 8;
    }
}