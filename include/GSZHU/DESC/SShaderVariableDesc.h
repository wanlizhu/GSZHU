#pragma once

#include <GSZHU/ENUM/EShaderVariableType.h>
#include <GSZHU/BasicTools.h>

namespace GSZHU {
    struct GSZHU_API SShaderVariableDesc {
        const char* Name = nullptr;
        ESHADER_VARIABLE_TYPE Type = SHADER_VARIABLE_TYPE_STATIC;

        SShaderVariableDesc() noexcept;
        SShaderVariableDesc(const char* _Name, ESHADER_VARIABLE_TYPE _Type) noexcept;

        bool operator==(const SShaderVariableDesc& rhs) const;
    };
}

namespace std {
    template<>
    struct hash<GSZHU::SShaderVariableDesc> {
        size_t operator()(const GSZHU::SShaderVariableDesc& Desc) const {
            return GSZHU::ComputeHash(std::string(Desc.Name), static_cast<int>(Desc.Type));
        }
    };
}