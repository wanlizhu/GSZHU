#pragma once

#include "Core/Common.h"

namespace Wanlix
{
    enum class EKey : Uint
    {
        // ASCII values. Do not change them.
        Space = ' ',
        Apostrophe = '\'',
        Comma = ',',
        Minus = '-',
        Period = '.',
        Slash = '/',
        Key0 = '0',
        Key1 = '1',
        Key2 = '2',
        Key3 = '3',
        Key4 = '4',
        Key5 = '5',
        Key6 = '6',
        Key7 = '7',
        Key8 = '8',
        Key9 = '9',
        Semicolon = ';',
        Equal = '=',
        A = 'A',
        B = 'B',
        C = 'C',
        D = 'D',
        E = 'E',
        F = 'F',
        G = 'G',
        H = 'H',
        I = 'I',
        J = 'J',
        K = 'K',
        L = 'L',
        M = 'M',
        N = 'N',
        O = 'O',
        P = 'P',
        Q = 'Q',
        R = 'R',
        S = 'S',
        T = 'T',
        U = 'U',
        V = 'V',
        W = 'W',
        X = 'X',
        Y = 'Y',
        Z = 'Z',
        LeftBracket = '[',
        Backslash = '\\',
        RightBracket = ']',
        GraveAccent = '`',

        // Special keys
        Escape,
        Tab,
        Enter,
        Backspace,
        Insert,
        Del,
        Right,
        Left,
        Down,
        Up,
        PageUp,
        PageDown,
        Home,
        End,
        CapsLock,
        ScrollLock,
        NumLock,
        PrintScreen,
        Pause,
        F1,
        F2,
        F3,
        F4,
        F5,
        F6,
        F7,
        F8,
        F9,
        F10,
        F11,
        F12,
        Keypad0,
        Keypad1,
        Keypad2,
        Keypad3,
        Keypad4,
        Keypad5,
        Keypad6,
        Keypad7,
        Keypad8,
        Keypad9,
        KeypadDel,
        KeypadDivide,
        KeypadMultiply,
        KeypadSubtract,
        KeypadAdd,
        KeypadEnter,
        KeypadEqual,
        LeftShift,
        LeftControl,
        LeftAlt,
        LeftSuper, // Windows key on windows
        RightShift,
        RightControl,
        RightAlt,
        RightSuper, // Windows key on windows
        Menu,
    };

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