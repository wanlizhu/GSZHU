#pragma once

#include "GE2/Utilities.h"

namespace GE2::RHI
{
    enum class EKeyEventType
    {
        None,
        KeyDown,
        KeyUp,
        Char,
    };

    enum class EMouseEventType
    {
        None,
        LeftDown,
        LeftUp,
        MiddleDown,
        MiddleUp,
        RightDown,
        RightUp,
        Move,
        Wheel,
    };

    struct InputModifiers
    {
        bool shift = false;
        bool ctrl = false;
        bool alt = false;
    };

    struct KeyEvent
    {
        EKeyEventType type = EKeyEventType::None;
        int key = 0;
        InputModifiers mods;
        uint32_t codepoint = 0;
    };

    struct MouseEvent
    {
        EMouseEventType type = EMouseEventType::None;
        FLOAT2 pos = { 0.f }; // Normalized coordinates x,y in range [0, 1]. (0,0) is the top-left corner of the window.
        FLOAT2 wheelDelta = { 0.f };
        InputModifiers mods;
    };
}