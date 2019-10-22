#pragma once

#include "GE2/BasicTypes.h"
#include "GE2/Utilities.h"
#include "ForwardDeclarations.h"

namespace GE2::RHI
{
    enum class EPolygonMode
    {
        Fill,
        Line,
        Point,
    };

    enum class ECullModeFlags
    {
        Front,
        Back,
        FrontAndBack,
    };

    enum class EFrontFace
    {
        CCW,
        CW
    };

    struct GE2_IMPEXP RasterizationState
    {
        bool           depthClampEnable = false;
        bool           rasterizerDiscardEnable = false;
        EPolygonMode   polygonMode = EPolygonMode::Fill;
        ECullModeFlags cullMode = ECullModeFlags::Back;
        EFrontFace     frontFace = EFrontFace::CCW;
        bool           depthBiasEnable = false;
        float          depthBiasConstantFactor = 0.f;
        float          depthBiasClamp = 0.f;
        float          depthBiasSlopeFactor = 0.f;
        float          lineWidth = 1.f;
    };
}