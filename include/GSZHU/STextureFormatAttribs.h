#pragma once

#include <GSZHU/ENUM/ETextureFormat.h>
#include <GSZHU/ENUM/EComponentType.h>

namespace GSZHU {
    struct GSZHU_API STextureFormatAttribs {
        const char* Name;
        ETEXTURE_FORMAT Format;
        uint8_t ComponentSize;
        uint8_t NumComponents;
        ECOMPONENT_TYPE ComponentType;
        bool IsTypeless;
        uint8_t BlockWidth;
        uint8_t BlockHeight;

        STextureFormatAttribs() noexcept;
        STextureFormatAttribs(const char* _Name, 
                              ETEXTURE_FORMAT _Format,
                              uint8_t _ComponentSize,
                              uint8_t _NumComponents,
                              ECOMPONENT_TYPE _ComponentType,
                              bool _IsTypeless, 
                              uint8_t _BlockWidth, 
                              uint8_t _BlockHeight) noexcept ;
    };
}