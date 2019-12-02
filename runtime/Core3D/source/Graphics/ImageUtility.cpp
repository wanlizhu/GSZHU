#include "ImageUtility.h"

namespace Wanlix
{
    ByteBuffer GenerateByteBuffer(size_t size, bool init)
    {
        auto buffer = std::unique_ptr<char[]>(new char[size]);

        if (init) {
            std::fill(buffer.get(), buffer.get() + size, 0);
        }

        return buffer;
    }

    ByteBuffer GenerateImageBuffer(
        const Extent& extent,
        const ColorFormat& format,
        const DataType& dataType,
        const Color4F& fillColor
    )
    {
        /* Allocate image buffer */
        const auto imageSize = extent.width * extent.height * extent.depth;
        const auto bytesPerPixel = DataTypeSize(dataType) * ColorFormatSize(format);
        auto imageBuffer = std::unique_ptr<char[]>(new char[bytesPerPixel * imageSize]);

        /* Initialize image buffer with fill color */
        
        for (size_t i = 0; i < imageSize; ++i) {
            std::memcpy(imageBuffer.get() + bytesPerPixel * i, &color, bytesPerPixel);
        }

        return imageBuffer;
    }
}