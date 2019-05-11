#pragma once

#include <GSZHU/BasicTypes.h>

namespace GSZHU {
    enum ESHADER_PROFILE : UINT8 {
        SHADER_PROFILE_DEFAULT = 0,
        SHADER_PROFILE_DX_4_0,
        SHADER_PROFILE_DX_5_0,
        SHADER_PROFILE_DX_5_1,
        SHADER_PROFILE_GL_4_2
    };
}