#pragma once

#include "Wanlix/Core3D/Graphics/Format.h"
#include "Wanlix/Core3D/Graphics/Color.h"
#include "TextureDescriptor.h"
#include <memory>
#include <cstdint>

namespace Wanlix
{
    using ByteBuffer = std::unique_ptr<char[]>;

    struct MutableImageDescriptor
    {
        ImageFormat format = ImageFormat::RGBA;
        DataType dataType = DataType::UInt8;
        void* data = nullptr;
        size_t dataSize = 0; /* This is primarily used for compressed images and serves for robustness. */

        MutableImageDescriptor() = default;
        MutableImageDescriptor(
            const ImageFormat& format,
            const DataType& dataType,
            void* data,
            size_t dataSize
        )
            : format(format)
            , dataType(dataType)
            , data(data)
            , dataSize(dataSize)
        {}
    };

    struct ImmutableImageDescriptor
    {
        ImageFormat format = ImageFormat::RGBA;
        DataType dataType = DataType::UInt8;
        const void* data = nullptr;
        size_t dataSize = 0; /* This is primarily used for compressed images and serves for robustness. */

        ImmutableImageDescriptor() = default;
        ImmutableImageDescriptor(
            const ImageFormat& format,
            const DataType& dataType,
            const void* data,
            size_t dataSize
        )
            : format(format)
            , dataType(dataType)
            , data(data)
            , dataSize(dataSize)
        {}
    };


    /* ----- Functions ----- */


    uint32_t ImageFormatSize(const ImageFormat& imageFormat); 
    uint32_t ImageDataSize(
        const ImageFormat imageFormat, 
        const DataType dataType, 
        uint32_t numPixels
    ); 

    bool IsCompressedFormat(const ImageFormat imageFormat);
    bool IsDepthStencilFormat(const ImageFormat imageFormat);

    bool ConvertImageBuffer(
        const ImmutableImageDescriptor& srcImageDesc,
        const MutableImageDescriptor& dstImageDesc,
        size_t threadCount = 0
    );
    ByteBuffer ConvertImageBuffer(
        const ImmutableImageDescriptor& srcImageDesc,
        ImageFormat dstFormat,
        DataType dstDataType,
        std::size_t threadCount = 0
    );
    ByteBuffer GenerateByteBuffer(
        size_t bufferSize,
        bool initialize = true
    );
    ByteBuffer GenerateImageBuffer(
        ImageFormat format,
        DataType dataType,
        size_t imageSize,
        const ColorRGBA<float>& fillColor
    );
    void CopyImageBufferRegion(
        // Destination
        const MutableImageDescriptor& dstImageDesc,
        const Offset& dstOffset,
        uint32_t dstRowStride,
        uint32_t dstSliceStride,

        // Source
        const ImmutableImageDescriptor& srcImageDesc,
        const Offset& srcOffset,
        uint32_t srcRowStride,
        uint32_t srcSliceStride,

        // Region
        const Extent& extent
    );
}