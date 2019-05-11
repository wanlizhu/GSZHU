#pragma once

#include <GSZHU/ENUM/EShaderVariableType.h>

namespace GSZHU {
    struct SShaderVariableDesc {
        const char* Name = nullptr;
        ESHADER_VARIABLE_TYPE Type = SHADER_VARIABLE_TYPE_STATIC;

        SShaderVariableDesc() noexcept {}
        SShaderVariableDesc(const char* _Name, ESHADER_VARIABLE_TYPE _Type) 
            : Name(_Name)
            , Type(_Type)
        {}
    };
}