#pragma once

#include <GSZHU/SDeviceObjectAttribs.h>
#include <GSZHU/SBufferFormat.h>
#include <GSZHU/ENUM/EBufferViewType.h>

namespace GSZHU {
    struct SBufferViewDesc : public SDeviceObjectAttribs {
        EBUFFER_VIEW_TYPE ViewType = BUFFER_VIEW_UNDEFINED;
        SBufferFormat Format;
        UINT ByteOffset = 0;
        UINT ByteWidth = 0;

        SBufferViewDesc() noexcept {}
        explicit SBufferViewDesc(EBUFFER_VIEW_TYPE _ViewType) noexcept
            : ViewType(_ViewType)
        {}

        bool operator==(const SBufferViewDesc& rhs) const {
            return ViewType == rhs.ViewType
                && ByteOffset == rhs.ByteOffset
                && ByteWidth == rhs.ByteWidth
                && Format == rhs.Format;
        }
    };
}