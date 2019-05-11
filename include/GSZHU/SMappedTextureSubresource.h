#pragma once

#include <GSZHU/BasicTypes.h>

namespace GSZHU {
    struct SMappedTextureSubresource {
        void* Data = nullptr;
        UINT Stride = 0;
        UINT DepthStride = 0;
    };
}