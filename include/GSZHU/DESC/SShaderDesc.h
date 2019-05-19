#pragma once

#include <GSZHU/DESC/SShaderVariableDesc.h>
#include <GSZHU/DESC/SStaticSamplerDesc.h>
#include <GSZHU/ENUM/EShaderVariableType.h>
#include <GSZHU/ENUM/EShaderProfile.h>
#include <GSZHU/ENUM/EShaderType.h>
#include <GSZHU/BasicTools.h>

namespace GSZHU {
    struct SShaderDesc : SDeviceObjectAttribs {
        ESHADER_TYPE ShaderType = SHADER_TYPE_VERTEX;
        bool CacheCompiledShader = false;
        ESHADER_PROFILE ShaderProfile = SHADER_PROFILE_DEFAULT;
        ESHADER_VARIABLE_TYPE DefaultVariableType = SHADER_VARIABLE_TYPE_STATIC;
        const SShaderVariableDesc* VariableArray = nullptr;
        uint32_t NumVariables = 0;
        const SStaticSamplerDesc* StaticSamplerArray = nullptr;
        uint32_t NumStaticSamplers = 0;

        SShaderDesc() noexcept;
        bool operator==(const SShaderDesc& rhs) const;
    };
}

namespace std {
    template<>
    struct hash<GSZHU::SShaderDesc> {
        size_t operator()(const GSZHU::SShaderDesc& Desc) const {
            return GSZHU::ComputeHash(static_cast<int>(Desc.ShaderType),
                static_cast<int>(Desc.CacheCompiledShader),
                static_cast<int>(Desc.ShaderProfile),
                static_cast<int>(Desc.DefaultVariableType),
                GSZHU::ComputeArrayHash<GSZHU::SShaderVariableDesc>(Desc.VariableArray, Desc.NumVariables),
                GSZHU::ComputeArrayHash<GSZHU::SStaticSamplerDesc>(Desc.StaticSamplerArray, Desc.NumStaticSamplers));
        }
    };
}