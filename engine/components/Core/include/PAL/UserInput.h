#pragma once

#include "Wanlix/Common.h"

namespace Wanlix
{
    struct InputModifiers
    {
        bool ctrl = false;  
        bool shift = false; 
        bool alt = false;   
    };

    struct MouseEvent
    {
        enum class EType
        {
            LeftButtonDown,         
            LeftButtonUp,           
            MiddleButtonDown,       
            MiddleButtonUp,         
            RightButtonDown,        
            RightButtonUp,          
            Move,                   
            Wheel                   
        };

        EType type;         
        Float2 pos;         
        Float2 screenPos;   
        Float2 wheelDelta;  
        InputModifiers mods;
    };

    struct KeyEvent
    {
        enum class EType
        {
            Pressed,    
            Released,   
            Input          
        };

        EType type;            
        EKey  key;             
        InputModifiers mods;   
        Uint codepoint = 0;
    };
}