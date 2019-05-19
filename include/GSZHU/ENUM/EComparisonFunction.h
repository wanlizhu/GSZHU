#pragma once

#include <GSZHU/BasicTypes.h>

namespace GSZHU {
    // This enumeartion is used by SamplerDesc, StencilOpDesc and DepthStencilStateDesc 
    enum ECOMPARISON_FUNCTION : uint8_t {
        COMPARISON_FUNC_UNKNOWN = 0,
        COMPARISON_FUNC_NEVER,
        COMPARISON_FUNC_LESS,
        COMPARISON_FUNC_EQUAL,
        COMPARISON_FUNC_LESS_EQUAL,
        COMPARISON_FUNC_GREATER,
        COMPARISON_FUNC_NOT_EQUAL,
        COMPARISON_FUNC_GREATER_EQUAL,
        COMPARISON_FUNC_ALWAYS,

        COMPARISON_FUNC_NUM_FUNCTIONS
    };
}