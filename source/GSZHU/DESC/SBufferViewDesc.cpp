#include <GSZHU/DESC/SBufferViewDesc.h>

namespace GSZHU {
    SBufferViewDesc::SBufferViewDesc() noexcept
    {}

    SBufferViewDesc::SBufferViewDesc(EBUFFER_VIEW_TYPE _ViewType,
                                     SBufferFormat _Format,
                                     uint32_t _ByteOffset,
                                     uint32_t _ByteWidth) noexcept
        : ViewType(_ViewType)
        , Format(_Format)
        , ByteOffset(_ByteOffset)
        , ByteWidth(_ByteWidth)
    {}

    bool SBufferViewDesc::operator==(const SBufferViewDesc& rhs) const {
        return ViewType == rhs.ViewType
            && ByteOffset == rhs.ByteOffset
            && ByteWidth == rhs.ByteWidth
            && Format == rhs.Format;
    }
}