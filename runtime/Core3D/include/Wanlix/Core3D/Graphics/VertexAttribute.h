#pragma once

#include "Types.h"

namespace Wanlix
{
    struct VertexAttribute
    {
        String      name;
        Format      format = Format::RGBA32Float;
        uint32_t    location = 0;
        uint32_t    semanticIndex = 0;
        SystemValue systemValue = SystemValue::Undefined;
        uint32_t    slot = 0;
        uint32_t    offset = 0;
        uint32_t    stride = 0;
        uint32_t    instanceDivisor = 0;
    };

}