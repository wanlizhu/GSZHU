#pragma once

#include <GSZHU/BasicTypes.h>

namespace GSZHU {
    // The enumeration is used by TextureDesc to describe misc texture flags
    enum EMISC_TEXTURE_FLAGS : UINT8 {
        MISC_TEXTURE_FLAG_NONE = 0x00,
        MISC_TEXTURE_FLAG_GENERATE_MIPS = 0x01
    };
    DEFINE_FLAG_ENUM_OPS(EMISC_TEXTURE_FLAGS)
}