#pragma once

#include <GSZHU/BasicTypes.h>

namespace GSZHU {
    enum ESHADER_VARIABLE_TYPE : uint8_t {
        SHADER_VARIABLE_TYPE_STATIC = 0,
        SHADER_VARIABLE_TYPE_MUTABLE,
        SHADER_VARIABLE_TYPE_DYNAMIC,
        SHADER_VARIABLE_TYPE_NUM_TYPES
    };
}