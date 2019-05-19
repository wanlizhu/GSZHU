#pragma once

#include <GSZHU/ENUM/ETextureFormat.h>
#include <GSZHU/ENUM/EComponentType.h>

namespace GSZHU {
    struct STextureFormatAttribs {
        const char* Name;
        ETEXTURE_FORMAT Format;
        uint8_t ComponentSize;
        uint8_t NumComponents;
        ECOMPONENT_TYPE ComponentType;
        bool IsTypeless;
        uint8_t BlockWidth;
        uint8_t BlockHeight;

        STextureFormatAttribs() noexcept
            : Name("TEX_FORMAT_UNKNOWN")
            , Format(TEX_FORMAT_UNKNOWN)
            , ComponentSize(0)
            , NumComponents(0)
            , ComponentType(COMPONENT_TYPE_UNDEFINED)
            , IsTypeless(false)
            , BlockWidth(0)
            , BlockHeight(0)
        {}
        STextureFormatAttribs(const char* _Name, ETEXTURE_FORMAT _Format,
                              uint8_t _ComponentSize, uint8_t _NumComponents, ECOMPONENT_TYPE _ComponentType,
                              bool _IsTypeless, uint8_t _BlockWidth, uint8_t _BlockHeight) noexcept
            : Name(_Name)
            , Format(_Format)
            , ComponentSize(_ComponentSize)
            , NumComponents(_NumComponents)
            , ComponentType(_ComponentType)
            , IsTypeless(_IsTypeless)
            , BlockWidth(_BlockWidth)
            , BlockHeight(_BlockHeight)
        {}
    };
}