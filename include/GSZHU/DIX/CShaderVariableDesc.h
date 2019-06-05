#pragma once

#include "IResourceDesc.h"

namespace GSZHU {
    class CShaderVariableDesc : public IResourceDesc {
    public:

    private:
        STRING mName;
        EShaderVariable mVariableType;
    };
}