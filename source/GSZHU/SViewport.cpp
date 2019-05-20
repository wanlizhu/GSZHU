#include <GSZHU/SViewport.h>

namespace GSZHU {
    SViewport::SViewport() noexcept 
    {}

    SViewport::SViewport(float _TopLeftX, float _TopLeftY, float _Width, float _Height) noexcept 
        : TopLeftX(_TopLeftX)
        , TopLeftY(_TopLeftY)
        , Width(_Width)
        , Height(_Height)
    {}
}