#pragma once

#include "Common.h"

namespace Wanlix
{
    struct SamplerCapability
    {
        Bool borderSamplingModeSupported = true;
        Bool anisotropicFilteringSupported = true;
        Bool lodBiasSupported = true;
    };

    struct TextureCapability
    {
        Bool texture1DSupported = true;
        Bool texture1DArraySupported = true;
        Bool texture2DMSSupported = true;
        Bool texture2DMSArraySupported = true;
        Bool textureViewSupported = true;
        Bool cubemapArraysSupported = true;
    };

    struct NDCAttribs
    {
        const float minZ;          
        const float ZtoDepthScale; 
        const float YtoVScale;     
    };

    struct DeviceCapability
    {
        DeviceType deviceType = DeviceType::Undefined;
        Int  majorVersion = 0;
        Int  minorVersion = 0;
        Bool separableProgramSupported = true;
        Bool indirectRenderingSupported = true;
        Bool wireframeFillSupported = true;
        Bool multithreadedResourceCreationSupported = false;
        Bool computeShadersSupported = true;
        Bool geometryShadersSupported = true;
        Bool tessellationSupported = true;
        SamplerCapability sampler;
        TextureCapability texture;
        NDCAttribs ndc;
    };
}