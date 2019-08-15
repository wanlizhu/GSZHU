#pragma once

#include "Graphics/Constants.h"

namespace ZHU
{
    struct StencilOperation
    {
        EComparisonFunction StencilFunction = EComparisonFunction::Always;
        EStencilOperation StencilFailOperation;
        EStencilOperation StencilDepthFailOperation;
        EStencilOperation StencilPassOperation;
    };


    struct DepthStencilStateDesc
    {
        bool EnableDepth = true;
        bool EnableDepthWrite = true;
        bool EnableStencil = true;

        EComparisonFunction DepthFunction = EComparisonFunction::Less;
        uint8_t StencilReadMask = 0xff;
        uint8_t StencilWriteMask = 0xff;
        StencilOperation FrontFaceStencil;
        StencilOperation BackFaceStencil;
    };
}