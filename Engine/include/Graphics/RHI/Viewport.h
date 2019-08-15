#pragma once

#include "Graphics/Constants.h"

namespace ZHU
{
    struct Viewport
    {
        float StartPoint[2] = { 0.f, 0.f }; // Top-left
        float Width = 0.f;
        float Height = 0.f;

        float MinDepth = 0.f;
        float MaxDepth = 1.f;
    };
}