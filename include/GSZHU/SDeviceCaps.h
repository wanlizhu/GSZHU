#pragma once

#include <GSZHU/ENUM/EDeviceType.h>

namespace GSZHU {
    struct GSZHU_API SSamplerCaps {
        bool BorderSamplingModeSupported = true;
        bool AnisotropicFilteringSupported = true;
        bool LODBiasSupported = true;
    };

    struct GSZHU_API STextureCaps {
        bool Texture1DSupported = true;
        bool Texture1DArraySupported = true;
        bool Texture2DMSSupported = true;
        bool Texture2DMSArraySupported = true;
        bool TextureViewSupported = true;
        bool CubemapArraysSupported = true;
    };

    struct GSZHU_API SDeviceCaps {
        struct NDCAttribs {
            const float MinZ;          // Minimum z value of normalized device coordinate space
            const float ZtoDepthScale; // NDC z to depth scale
            const float YtoVScale;     // Scale to transform NDC y coordinate to texture V coordinate

            float GetZtoDepthBias() const;
        };

        EDeviceType DeviceType = EDeviceType::UNDEFINED;
        int MajorVersion = 0;
        int MinorVersion = 0;
        bool SeparableProgramSupported = true;
        bool IndirectRenderingSupported = true;
        bool WireframeFillSupported = true;
        bool MultithreadedResourceCreationSupported = false;
        bool ComputeShadersSupported = true;
        bool GeometryShadersSupported = true;
        bool TessellationSupported = true;
        SSamplerCaps SamplerCaps;
        STextureCaps TextureCaps;

        bool IsGLDevice() const;
        bool IsD3DDevice() const;
        bool IsVulkanDevice() const;
        const NDCAttribs& GetNDCAttribs() const;
    };
}