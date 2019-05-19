#include <GSZHU/DESC/SShaderVariableDesc.h>
#include <string>

namespace GSZHU {
    SShaderVariableDesc::SShaderVariableDesc() noexcept
    {}

    SShaderVariableDesc::SShaderVariableDesc(const char* _Name, ESHADER_VARIABLE_TYPE _Type) noexcept
        : Name(_Name)
        , Type(_Type)
    {}

    bool SShaderVariableDesc::operator==(const SShaderVariableDesc& rhs) const {
        return strcmp(Name, rhs.Name) == 0 
            && Type == rhs.Type;
    }
}