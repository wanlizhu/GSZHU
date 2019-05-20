#include <GSZHU/SShaderMacro.h>

namespace GSZHU {
    SShaderMacro::SShaderMacro() noexcept 
    {}

    SShaderMacro::SShaderMacro(const char* _Name, const char* _Def) noexcept 
        : Name(_Name)
        , Definition(_Def)
    {}
}