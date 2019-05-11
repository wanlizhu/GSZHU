#pragma once

#include <GSZHU/ENUM/EDeviceType.h>

namespace GSZHU {
    struct SSamplerCaps {
        bool BorderSamplingModeSupported = true;
        bool AnisotropicFilteringSupported = true;
        bool LODBiasSupported = true;
    };

    struct STextureCaps {
        bool Texture1DSupported = true;
        bool Texture1DArraySupported = true;
        bool Texture2DMSSupported = true;
        bool Texture2DMSArraySupported = true;
        bool TextureViewSupported = true;
        bool CubemapArraysSupported = true;
    };

    struct SDeviceCaps {
        struct NDCAttribs {
            const float MinZ;          // Minimum z value of normalized device coordinate space
            const float ZtoDepthScale; // NDC z to depth scale
            const float YtoVScale;     // Scale to transform NDC y coordinate to texture V coordinate

            float GetZtoDepthBias() const;
        };

        EDeviceType DeviceType = EDeviceType::UNDEFINED;
        INT MajorVersion = 0;
        INT MinorVersion = 0;
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