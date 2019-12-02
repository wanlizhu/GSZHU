#pragma once

#include "Wanlix/Core3D/Graphics/Types.h"

namespace Wanlix
{
    ByteBuffer GenerateImageBuffer(
        const Extent& extent,
        const ColorFormat& format,
        const DataType& dataType,
        const Color4F& fillColor
    );
}