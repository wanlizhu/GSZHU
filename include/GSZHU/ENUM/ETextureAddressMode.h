#pragma once

#include <GSZHU/BasicTypes.h>

namespace GSZHU {
    // Defines a technique for resolving texture coordinates that are outside of the boundaries of a texture. 
    enum ETEXTURE_ADDRESS_MODE : UINT8 {
        TEXTURE_ADDRESS_UNKNOWN = 0,
        TEXTURE_ADDRESS_WRAP = 1,
        TEXTURE_ADDRESS_MIRROR = 2,
        TEXTURE_ADDRESS_CLAMP = 3,
        TEXTURE_ADDRESS_BORDER = 4,
        TEXTURE_ADDRESS_MIRROR_ONCE = 5,

        TEXTURE_ADDRESS_NUM_MODES
    };
}