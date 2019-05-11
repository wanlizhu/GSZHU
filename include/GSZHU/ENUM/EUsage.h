#pragma once

#include <GSZHU/BasicTypes.h>

namespace GSZHU {
    // The enumeration is used by BufferDesc and TextureDesc
    enum EUSAGE : UINT8 {
        USAGE_STATIC = 0,
        USAGE_DEFAULT,
        USAGE_DYNAMIC,
        USAGE_CPU_ACCESSIBLE
    };
}