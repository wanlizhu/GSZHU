#pragma once

#include <GSZHU/BasicTypes.h>
#include <GSZHU/STextureFormatAttribs.h>

namespace GSZHU {
    struct STextureFormatInfo : public STextureFormatAttribs {
        bool Supported = false; // Indicates if the format is supported by the device
    };

    struct STextureFormatInfoEXT : public STextureFormatInfo {
        bool Filterable = false;
        bool ColorRenderable = false;
        bool DepthRenderable = false;
        bool Tex1DFmt = false;
        bool Tex2DFmt = false;
        bool Tex3DFmt = false;
        bool TexCubeFmt = false;
        uint32_t SampleCounts = 0;
    };

}