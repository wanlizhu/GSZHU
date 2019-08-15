#pragma once

#include "Graphics/Constants.h"

namespace ZHU
{
    struct DeviceCaps
    {
        EDevice DeviceType;
        int MajorVersion = 0;
        int MinorVersion = 0;
        bool SeparableProgram = true;
        bool IndirectRendering = true;
        bool WireframeFill = true;
        bool MultithreadedResourceCreation = true;
        bool ComputeShader = true;
        bool GeometryShader = true;
        bool Tessellation = true;
        
        // Sampler Caps
        bool BorderSamplingMode = true;
        bool AnisotropicFiltering = true;
        bool LODBias = true;

        // Texture Caps
        bool Texture1D = true;
        bool Texture1DArray = true;
        bool Texture2DMS = true;
        bool Texture2DMSArray = true;
        bool TextureView = true;
        bool CubemapArray = true;
    };
}