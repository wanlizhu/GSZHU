#include "Wanlix/Core3D/Graphics/RHI/Image.h"

namespace Wanlix
{
    Image::Image(
        const Extent&      extent,
        const ColorFormat& format,
        const DataType&    dataType,
        ByteBuffer&&       data,
        const Color4F&     fillColor
    ) noexcept
        : mExtent(extent)
        , mFormat(format)
        , mDataType(dataType)
        , mData(std::move(data))
    {
        if (!mData) {
            if (fillColor.IsNull()) {
                mData = GenerateByteBuffer(GetDataSize());
            }
            else {
                mData = std::move(GenerateImageBuffer(format, dataType, GetNumPixels(), fillColor));
            }
        }
    }

    Image::Image(const Image& rhs) noexcept
    {

    }

    Image::Image(Image&& rhs) noexcept
    {

    }
}