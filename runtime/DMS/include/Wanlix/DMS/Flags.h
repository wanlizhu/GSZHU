#pragma once

#include "Wanlix/Flags.h"

namespace Wanlix
{
    struct Primitive
    {
        enum {
            Unknown = 0,
            Point = 1,
            Line = (1 << 1),
            Triangle = (1 << 2),
            Polygon = (1 << 3),
        };
    };

    enum class TextureMapping
    {
        UV = 0,
        Sphere,
        Cylinder,
        Box,
        Plane,
    };

    enum class TextureType
    {
        None = 0,
        Diffuse,
        Specular,
        Ambient,
        Emissive,
        Height,
        Normals,
        Shininess,
        Opacity,
        Displacement,
        LightMap,
        Reflection,

        PBR_BaseColor,
        PBR_Normal,
        PBR_EmissionColor,
        PBR_Metalness,
        PBR_Roughness,
        PBR_AmbientOcclusion,
    };

    enum class ShadingMode
    {
        None = 0,
        Flat,
        Gourand,
        Phong,
        PhongBlinn,
        Comic,
        OrenNayar,
        Minnaert,
        CookTorrance,
        Fresnel,
    };

    enum class LightType
    {
        Undefined = 0,
        Directional,
        Point,
        Spot,
        Ambient,
        Area,
    };
}