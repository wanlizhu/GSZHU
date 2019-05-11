#pragma once

#include <GSZHU/BasicTypes.h>
#include <GSZHU/ENUM/ETextureFormat.h>

namespace GSZHU {
    struct SSwapChainDesc {
        UINT Width = 0;
        UINT Height = 0;
        ETEXTURE_FORMAT ColorBufferFormat = TEX_FORMAT_RGBA8_UNORM_SRGB;
        ETEXTURE_FORMAT DepthBufferFormat = TEX_FORMAT_D32_FLOAT;
        UINT SamplesCount = 1;
        UINT BufferCount = 2;
        float DefaultDepthValue = 1.f;
        UINT8 DefaultStencilValue = 0;

        SSwapChainDesc() noexcept {}
        SSwapChainDesc(UINT         _Width,
            UINT         _Height,
            ETEXTURE_FORMAT _ColorBufferFormat,
            ETEXTURE_FORMAT _DepthBufferFormat) 
            : Width(_Width)
            , Height(_Height)
            , ColorBufferFormat(_ColorBufferFormat)
        {}
    };
}