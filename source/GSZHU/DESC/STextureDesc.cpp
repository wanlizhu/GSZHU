#include <GSZHU/DESC/STextureDesc.h>

namespace GSZHU {
    STextureDesc::STextureDesc() noexcept
    {}

    STextureDesc::STextureDesc(ERESOURCE_DIMENSION _Type,
                               uint32_t _Width, 
                               uint32_t _Height,
                               uint32_t _ArraySizeOrDepth,
                               ETEXTURE_FORMAT _Format) noexcept
        : Type(_Type)
        , Width(_Width)
        , Height(_Height)
        , ArraySize(_ArraySizeOrDepth)
        , Format(_Format)
    {}

    bool STextureDesc::operator==(const STextureDesc& rhs) const {
        return Type == rhs.Type
            && Width == rhs.Width
            && Height == rhs.Height
            && ArraySize == rhs.ArraySize
            && Format == rhs.Format
            && MipLevels == rhs.MipLevels
            && SampleCount == rhs.SampleCount
            && Usage == rhs.Usage
            && BindFlags == rhs.BindFlags
            && CPUAccessFlags == rhs.CPUAccessFlags 
            && MiscFlags == rhs.MiscFlags
            && ClearValue == rhs.ClearValue
            && CommandQueueMask == rhs.CommandQueueMask;
    }
}