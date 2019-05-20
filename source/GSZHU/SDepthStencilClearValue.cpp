#include <GSZHU/SDepthStencilClearValue.h>

namespace GSZHU {
    SDepthStencilClearValue::SDepthStencilClearValue() noexcept 
    {}

    SDepthStencilClearValue::SDepthStencilClearValue(float _Depth, uint8_t _Stencil) noexcept 
        : Depth(_Depth)
        , Stencil(_Stencil)
    {}
}