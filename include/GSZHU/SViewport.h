#pragma once

namespace GSZHU {
    // This structure is used by IDeviceContext::SetViewports().
    struct SViewport {
        float TopLeftX = 0.f;
        float TopLeftY = 0.f;
        float Width = 0.f;
        float Height = 0.f;
        float MinDepth = 0.f;
        float MaxDepth = 1.f;

        SViewport() noexcept {}
        SViewport(float _TopLeftX, float _TopLeftY, float _Width, float _Height) noexcept 
            : TopLeftX(_TopLeftX)
            , TopLeftY(_TopLeftY)
            , Width(_Width)
            , Height(_Height)
        {}
    };
}