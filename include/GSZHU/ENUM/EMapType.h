#pragma once

#include <GSZHU/BasicTypes.h>

namespace GSZHU {
    enum EMAP_TYPE : uint8_t {
        MAP_READ = 0x01,
        MAP_WRITE = 0x02,
        MAP_READ_WRITE = 0x03
    };
}