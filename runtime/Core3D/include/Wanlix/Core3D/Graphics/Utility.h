#pragma once

#include "RHI/Image.h"
#include "RHI/Texture.h"

namespace Wanlix
{
    ByteBuffer GenerateByteBuffer(size_t bytes);

    ByteBuffer GenerateImageBuffer(
        const ColorFormat& format,
        const DataType&    dataType,
        const Extent&      imageExtent,
        const Color4F&     fillColor
    );

    bool ConvertImageBuffer(
        const ImageReadDescriptor&  srcImageDesc,
        const ImageWriteDescriptor& dstImageDesc,
        int threads = 0
    );

    bool CopyImageBuffer(
        const ImageWriteDescriptor& dstImageDesc,
        const Region& dstRegion,
        const ImageReadDescriptor&  srcImageDesc,
        const Region& srcRegion
    );


}