#pragma once

namespace GSZHU {
    struct SShaderMacro {
        const char* Name = nullptr;
        const char* Definition = nullptr;

        SShaderMacro() noexcept {}
        SShaderMacro(const char* _Name, const char* _Def) noexcept 
            : Name(_Name)
            , Definition(_Def)
        {}
    };
}