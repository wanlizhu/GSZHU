#pragma once

#include "IResourceDesc.h"
#include "CShaderVariableDesc.h"

namespace GSZHU {
    class CShaderDesc : public IResourceDesc {
    public:

    private:
        EShader mShaderType = EShader::Unknown;
        std::vector<CShaderVariableDesc> mVariables;
    };
}