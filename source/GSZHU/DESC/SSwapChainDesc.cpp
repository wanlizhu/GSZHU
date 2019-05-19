#include <GSZHU/DESC/SSwapChainDesc.h>
#include <GSZHU/BasicTypes.h>

namespace GSZHU {
    SSwapChainDesc::SSwapChainDesc() noexcept
    {}

    SSwapChainDesc::SSwapChainDesc(uint32_t _Width,
                                   uint32_t _Height,
                                   ETEXTURE_FORMAT _ColorBufferFormat,
                                   ETEXTURE_FORMAT _DepthBufferFormat) noexcept
        : Width(_Width)
        , Height(_Height)
        , ColorBufferFormat(_ColorBufferFormat)
    {}

    bool SSwapChainDesc::operator==(const SSwapChainDesc& rhs) const {
        return Width == rhs.Width
            && Height == rhs.Height
            && ColorBufferFormat == rhs.ColorBufferFormat
            && DepthBufferFormat == rhs.DepthBufferFormat
            && SamplesCount == rhs.SamplesCount
            && BufferCount == rhs.BufferCount
            && FLT_EQ(DefaultDepthValue, rhs.DefaultDepthValue)
            && DefaultStencilValue == rhs.DefaultStencilValue;
    }
}