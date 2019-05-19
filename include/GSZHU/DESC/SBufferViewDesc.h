#pragma once

#include <GSZHU/SDeviceObjectAttribs.h>
#include <GSZHU/SBufferFormat.h>
#include <GSZHU/ENUM/EBufferViewType.h>
#include <GSZHU/BasicTools.h>

namespace GSZHU {
    struct SBufferViewDesc : public SDeviceObjectAttribs {
        EBUFFER_VIEW_TYPE ViewType = BUFFER_VIEW_UNDEFINED;
        SBufferFormat Format;
        uint32_t ByteOffset = 0;
        uint32_t ByteWidth = 0;

        SBufferViewDesc() noexcept;
        explicit SBufferViewDesc(EBUFFER_VIEW_TYPE _ViewType,
                                 SBufferFormat _Format = SBufferViewDesc{}.Format,
                                 uint32_t _ByteOffset = SBufferViewDesc{}.ByteOffset,
                                 uint32_t _ByteWidth = SBufferViewDesc{}.ByteWidth) noexcept   ;

        bool operator==(const SBufferViewDesc& rhs) const;
    };
}

namespace std {
    template<>
    struct hash<GSZHU::SBufferViewDesc> {
        size_t operator()(const GSZHU::SBufferViewDesc& Desc) const {
            return GSZHU::ComputeHash(static_cast<int>(Desc.ViewType), 
                                      Desc.ByteOffset,
                                      Desc.ByteWidth,
                                      std::hash<GSZHU::SBufferFormat>()(Desc.Format));
        }
    };
}