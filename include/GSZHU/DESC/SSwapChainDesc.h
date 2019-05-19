#pragma once

#include <GSZHU/BasicTypes.h>
#include <GSZHU/ENUM/ETextureFormat.h>

namespace GSZHU {
    struct SSwapChainDesc {
        uint32_t Width = 0;
        uint32_t Height = 0;
        ETEXTURE_FORMAT ColorBufferFormat = TEX_FORMAT_RGBA8_UNORM_SRGB;
        ETEXTURE_FORMAT DepthBufferFormat = TEX_FORMAT_D32_FLOAT;
        uint32_t SamplesCount = 1;
        uint32_t BufferCount = 2;
        float DefaultDepthValue = 1.f;
        uint8_t DefaultStencilValue = 0;

        SSwapChainDesc() noexcept {}
        SSwapChainDesc(uint32_t _Width,
                       uint32_t _Height,
                       ETEXTURE_FORMAT _ColorBufferFormat,
                       ETEXTURE_FORMAT _DepthBufferFormat) 
            : Width(_Width)
            , Height(_Height)
            , ColorBufferFormat(_ColorBufferFormat)
        {}

        bool operator==(const SSwapChainDesc& rhs) const;
    };
}

namespace std {
    template<>
    struct hash<GSZHU::SSwapChainDesc> {
        size_t operator()(const GSZHU::SSwapChainDesc& Desc) const {
            return GSZHU::ComputeHash(Desc.Width, 
                                      Desc.Height,
                                      static_cast<int>(Desc.ColorBufferFormat),
                                      static_cast<int>(Desc.DepthBufferFormat),
                                      Desc.SamplesCount,
                                      Desc.BufferCount,
                                      Desc.DefaultDepthValue,
                                      Desc.DefaultStencilValue);
        }
    };
}