#pragma once

#include "Wanlix/Core3D/Graphics/Types.h"

namespace Wanlix
{
    ByteBuffer GenerateByteBuffer(size_t size, bool init = true);
    ByteBuffer GenerateImageBuffer(
        const Extent& extent,
        const ColorFormat& format,
        const DataType& dataType,
        const Color4F& fillColor
    );
}