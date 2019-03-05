#pragma once

#include <NTGS/Graphics/ITextureView.h>
#include <NTGS/Graphics/GraphicsEnums.h>

namespace NTGS {
    struct DisplayModeAttribs {
        // Flags indicating how an image is stretched to fit a given monitor's resolution.
        enum SCALING {
            SCALING_UNSPECIFIED = 0,
            SCALING_CENTERED = 1,
            SCALING_STRETCHED = 2
        };

        // Flags indicating the method the raster uses to create an image on a surface.
        enum SCANLINE_ORDER {
            SCANLINE_ORDER_UNSPECIFIED = 0,
            SCANLINE_ORDER_PROGRESSIVE = 1,
            SCANLINE_ORDER_UPPER_FIELD_FIRST = 2,
            SCANLINE_ORDER_LOWER_FIELD_FIRST = 3
        };

        UINT Width = 0;
        UINT Height = 0;
        TEXTURE_FORMAT Format = TEX_FORMAT_UNKNOWN;

        UINT RefreshRateNumerator = 0;
        UINT RefreshRateDenominator = 0;

        SCALING Scaling = SCALING_UNSPECIFIED;
        SCANLINE_ORDER ScanlineOrder = SCANLINE_ORDER_UNSPECIFIED;
    };

    struct FullScreenModeDesc {
        bool Fullscreen = false;

        UINT RefreshRateNumerator = 0;
        UINT RefreshRateDenominator = 0;

        DisplayModeAttribs::SCALING Scaling = DisplayModeAttribs::SCALING_UNSPECIFIED;
        DisplayModeAttribs::SCANLINE_ORDER ScanlineOrder = DisplayModeAttribs::SCANLINE_ORDER_UNSPECIFIED;
    };

    struct SwapChainDesc {
        UINT Width = 0;
        UINT Height = 0;

        TEXTURE_FORMAT ColorBufferFormat = TEX_FORMAT_RGBA8_UNORM_SRGB;
        TEXTURE_FORMAT DepthBufferFormat = TEX_FORMAT_D32_FLOAT;

        UINT SamplesCount = 1;
        UINT BufferCount = 2;

        FLOAT DefaultDepthValue = 1.f;
        UINT8 DefaultStencilValue = 0;

        SwapChainDesc() = default;

        SwapChainDesc(UINT _Width,
            UINT _Height,
            TEXTURE_FORMAT _ColorBufferFormat,
            TEXTURE_FORMAT _DepthBufferFormat,
            UINT _SamplesCount = SwapChainDesc{}.SamplesCount,
            UINT _BufferCount = SwapChainDesc{}.BufferCount,
            FLOAT _DefaultDepthValue = SwapChainDesc{}.DefaultDepthValue,
            UINT8 _DefaultStencilValue = SwapChainDesc{}.DefaultStencilValue)
            : Width(_Width),
            Height(_Height),
            ColorBufferFormat(_ColorBufferFormat),
            DepthBufferFormat(_DepthBufferFormat),
            SamplesCount(_SamplesCount),
            BufferCount(_BufferCount),
            DefaultDepthValue(_DefaultDepthValue),
            DefaultStencilValue(_DefaultStencilValue) {}
    };

    class ISwapChain : public IObject {
    public:
        virtual void Present(UINT SyncInterval = 1) = 0;
        virtual const SwapChainDesc& GetDesc()const = 0;
        virtual void Resize(UINT NewWidth, UINT NewHeight) = 0;
        virtual void SetFullscreenMode(const DisplayModeAttribs& DisplayMode) = 0;
        virtual void SetWindowedMode() = 0;
        virtual ITextureView* GetCurrentBackBufferRTV() = 0;
        virtual ITextureView* GetDepthBufferDSV() = 0;
    };
}