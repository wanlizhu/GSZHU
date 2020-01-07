#pragma once

#include <cstdint>
#include <string>
#include "Wanlix/Utils/Flags.h"

namespace Wanlix
{
    // Used for GPU side operations. 
    enum class EPixelFormat
    {
        Undefined = 0,      

        /* --- Alpha channel color formats --- */
        A8UNorm,            

        /* --- Red channel color formats --- */
        R8UNorm,            
        R8SNorm,            
        R8UInt,             
        R8SInt,             

        R16UNorm,           
        R16SNorm,           
        R16UInt,            
        R16SInt,            
        R16Float,           

        R32UInt,            
        R32SInt,            
        R32Float,           

        R64Float,           //!< Only supported with: Vulkan.

        /* --- RG color formats --- */
        RG8UNorm,           
        RG8SNorm,           
        RG8UInt,            
        RG8SInt,            

        RG16UNorm,          
        RG16SNorm,          
        RG16UInt,           
        RG16SInt,           
        RG16Float,          

        RG32UInt,           
        RG32SInt,           
        RG32Float,          

        RG64Float,          //!< Only supported with: Vulkan.

        /* --- RGB color formats --- */
        RGB8UNorm,          //!< Only supported with: OpenGL, Vulkan.
        RGB8UNorm_sRGB,     //!< Only supported with: OpenGL, Vulkan.
        RGB8SNorm,          //!< Only supported with: OpenGL, Vulkan.
        RGB8UInt,           //!< Only supported with: OpenGL, Vulkan.
        RGB8SInt,           //!< Only supported with: OpenGL, Vulkan.

        RGB16UNorm,         //!< Only supported with: OpenGL, Vulkan.
        RGB16SNorm,         //!< Only supported with: OpenGL, Vulkan.
        RGB16UInt,          //!< Only supported with: OpenGL, Vulkan.
        RGB16SInt,          //!< Only supported with: OpenGL, Vulkan.
        RGB16Float,         //!< Only supported with: OpenGL, Vulkan.

        RGB32UInt,          //!< As texture format only supported with: OpenGL, Vulkan, Direct3D 11, Direct3D 12.
        RGB32SInt,          //!< As texture format only supported with: OpenGL, Vulkan, Direct3D 11, Direct3D 12.
        RGB32Float,         //!< As texture format only supported with: OpenGL, Vulkan, Direct3D 11, Direct3D 12.

        RGB64Float,         //!< Only supported with: Vulkan.

        /* --- RGBA color formats --- */
        RGBA8UNorm,         
        RGBA8UNorm_sRGB,    
        RGBA8SNorm,         
        RGBA8UInt,          
        RGBA8SInt,          

        RGBA16UNorm,        
        RGBA16SNorm,        
        RGBA16UInt,         
        RGBA16SInt,         
        RGBA16Float,        

        RGBA32UInt,         
        RGBA32SInt,         
        RGBA32Float,        

        RGBA64Float,        //!< Only supported with: Vulkan.

        /* --- BGRA color formats --- */
        BGRA8UNorm,         
        BGRA8UNorm_sRGB,    
        BGRA8SNorm,         //!< Only supported with: Vulkan.
        BGRA8UInt,          //!< Only supported with: Vulkan.
        BGRA8SInt,          //!< Only supported with: Vulkan.

        /* --- Packed formats --- */
        RGB10A2UNorm,       
        RGB10A2UInt,        
        RG11B10Float,       
        RGB9E5Float,        

        /* --- Depth-stencil formats --- */
        D16UNorm,           
        D24UNormS8UInt,     
        D32Float,           
        D32FloatS8X24UInt,  
        S8UInt,             //!< Only supported with: OpenGL, Vulkan, Metal.

        /* --- Block compression (BC) formats --- */
        BC1UNorm,           //!< S3TC BC1 compressed RGBA with normalized unsigned integer components in 64-bit per 4x4 block.
        BC1UNorm_sRGB,      //!< S3TC BC1 compressed RGBA with normalized unsigned integer components in 64-bit per 4x4 block in non-linear sRGB color space.
        BC2UNorm,           //!< S3TC BC2 compressed RGBA with normalized unsigned integer components in 128-bit per 4x4 block.
        BC2UNorm_sRGB,      //!< S3TC BC2 compressed RGBA with normalized unsigned integer components in 128-bit per 4x4 block in non-linear sRGB color space.
        BC3UNorm,           //!< S3TC BC3 compressed RGBA with normalized unsigned integer components in 128-bit per 4x4 block.
        BC3UNorm_sRGB,      //!< S3TC BC3 compressed RGBA with normalized unsigned integer components in 128-bit per 4x4 block in non-linear sRGB color space.
        BC4UNorm,           //!< S3TC BC4 compressed red channel with normalized unsigned integer component in 64-bit per 4x4 block.
        BC4SNorm,           //!< S3TC BC4 compressed red channel with normalized signed integer component 64-bit per 4x4 block.
        BC5UNorm,           //!< S3TC BC5 compressed red and green channels with normalized unsigned integer components in 64-bit per 4x4 block.
        BC5SNorm,           //!< S3TC BC5 compressed red and green channels with normalized signed integer components in 128-bit per 4x4 block.
    };

    enum class EImageFormat
    {
        /* Color formats */
        Alpha,          
        R,              
        RG,             
        RGB,            
        BGR,            
        RGBA,           
        BGRA,           
        ARGB,           //!< Old format, mainly used in Direct3D 9.
        ABGR,           //!< Old format, mainly used in Direct3D 9.

        /* Depth-stencil formats */
        Depth,        
        Stencil,
        DepthStencil,   

        /* Compressed formats */
        BC1,            
        BC2,            
        BC3,            
        BC4,            
        BC5,            
    };

    enum class EPixelFormatFlags
    {
        None = 0,
        HasDepth = (1 << 0),
        HasStencil = (1 << 1),
        IsColorSpace_sRGB = (1 << 2),
        IsCompressed = (1 << 3),
        IsNormalized = (1 << 4),
        IsInteger = (1 << 5),
        IsUnsigned = (1 << 6),
        IsPacked = (1 << 7),
        SupportsRenderTarget = (1 << 8),
        SupportsMips = (1 << 9),
        SupportsGenerateMips = (1 << 10),
        SupportsTexture1D = (1 << 11),
        SupportsTexture2D = (1 << 12),
        SupportsTexture3D = (1 << 13),
        SupportsTextureCube = (1 << 14),
        SupportsVertex = (1 << 15),
    };
    ENUM_CLASS_OPERATORS(EPixelFormatFlags)

    struct PixelFormatDesc
    {
        uint32_t bits;
        uint8_t  blockWidth;
        uint8_t  blockHeight;
        uint8_t  components;

        EImageFormat imageFormat;
        EDataType dataType;
        EPixelFormatFlags flags;

        bool IsCompressedFormat() const;
        bool IsDepthStencilFormat() const;
        bool IsDepthFormat() const;
        bool IsStencilFormat() const;
        bool IsNormalizedFormat() const;
        bool IsIntegralFormat() const;
        bool IsFloatFormat() const;
        uint32_t GetSizeInBytes() const;
    };


    PixelFormatDesc const& GetPixelFormatDesc(EPixelFormat format);
    PixelFormatDesc const& GetPixelFormatDesc(EImageFormat imageFormat, EDataType dataType);
}