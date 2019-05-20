#pragma once

#include <GSZHU/BasicTools.h>

namespace GSZHU {
    struct GSZHU_API SShaderMacro {
        const char* Name = nullptr;
        const char* Definition = nullptr;

        SShaderMacro() noexcept;
        SShaderMacro(const char* _Name, const char* _Def) noexcept;
    };
}