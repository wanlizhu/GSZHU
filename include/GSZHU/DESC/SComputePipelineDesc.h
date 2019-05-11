#pragma once

#include <GSZHU/IShader.h>

namespace GSZHU {
    struct SComputePipelineDesc {
        IShader* ShaderCS = nullptr;
    };
}