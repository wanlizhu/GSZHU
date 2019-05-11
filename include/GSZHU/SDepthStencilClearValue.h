#pragma once

#include <GSZHU/BasicTypes.h>

namespace GSZHU {
    struct SDepthStencilClearValue {
        float Depth = 1.f;
        UINT8 Stencil = 0;

        SDepthStencilClearValue() noexcept {}
        SDepthStencilClearValue(float _Depth, UINT8 _Stencil) noexcept 
            : Depth(_Depth)
            , Stencil(_Stencil)
        {}
    };
}