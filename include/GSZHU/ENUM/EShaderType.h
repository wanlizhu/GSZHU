#pragma once

#include <GSZHU/BasicTypes.h>

namespace GSZHU {
    enum ESHADER_TYPE : uint32_t {
        SHADER_TYPE_UNKNOWN = 0x000, 
        SHADER_TYPE_VERTEX = 0x001, 
        SHADER_TYPE_PIXEL = 0x002, 
        SHADER_TYPE_GEOMETRY = 0x004, 
        SHADER_TYPE_HULL = 0x008, 
        SHADER_TYPE_DOMAIN = 0x010, 
        SHADER_TYPE_COMPUTE = 0x020 
    };
}