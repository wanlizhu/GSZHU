#pragma once

#include <GSZHU/DESC/SShaderVariableDesc.h>
#include <GSZHU/DESC/SStaticSamplerDesc.h>
#include <GSZHU/ENUM/EShaderVariableType.h>
#include <GSZHU/ENUM/EShaderProfile.h>
#include <GSZHU/ENUM/EShaderType.h>

namespace GSZHU {
    struct SShaderDesc : SDeviceObjectAttribs {
        ESHADER_TYPE ShaderType = SHADER_TYPE_VERTEX;
        bool CacheCompiledShader = false;
        ESHADER_PROFILE ShaderProfile = SHADER_PROFILE_DEFAULT;
        ESHADER_VARIABLE_TYPE DefaultVariableType = SHADER_VARIABLE_TYPE_STATIC;
        const SShaderVariableDesc* VariableDesc = nullptr;
        UINT NumVariables = 0;
        UINT NumStaticSamplers = 0;
        const SStaticSamplerDesc* StaticSamplers = nullptr;
    };
}