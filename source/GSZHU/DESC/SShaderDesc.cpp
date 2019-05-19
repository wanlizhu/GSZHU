#include <GSZHU/DESC/SShaderDesc.h>

namespace GSZHU {
    SShaderDesc::SShaderDesc() noexcept
    {}

    bool SShaderDesc::operator==(const SShaderDesc& rhs) const {
        if(!(ShaderType == rhs.ShaderType)
            || !(CacheCompiledShader == rhs.CacheCompiledShader)
            || !(ShaderProfile == rhs.ShaderProfile)
            || !(DefaultVariableType == rhs.DefaultVariableType)
            || !(NumVariables == rhs.NumVariables)
            || !(NumStaticSamplers == rhs.NumStaticSamplers)) {
            return false;
        }

        for (int i = 0; i < NumVariables; i++) {
            if (!(VariableArray[i] == rhs.VariableArray[i]))
                return false;
        }

        for (int i = 0; i < NumStaticSamplers; i++) {
            if (!(StaticSamplerArray[i] == rhs.StaticSamplerArray[i]))
                return false;
        }

        return true;
    }
}