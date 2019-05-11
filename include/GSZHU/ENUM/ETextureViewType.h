#pragma once

#include <GSZHU/BasicTypes.h>

namespace GSZHU {
    // This enumeration describes allowed view types for a texture view. It is used by TextureViewDesc structure.
    enum ETEXTURE_VIEW_TYPE : UINT8 {
        TEXTURE_VIEW_UNDEFINED = 0,
        TEXTURE_VIEW_SHADER_RESOURCE,
        TEXTURE_VIEW_RENDER_TARGET,
        TEXTURE_VIEW_DEPTH_STENCIL,
        TEXTURE_VIEW_UNORDERED_ACCESS,

        TEXTURE_VIEW_NUM_VIEWS
    };
}