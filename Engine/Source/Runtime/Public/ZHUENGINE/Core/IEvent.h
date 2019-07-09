#pragma once

#include <ZHUENGINE/Core/Object.h>

namespace ZHU
{
    class IEvent 
    {
    public:
        enum class EDevice 
        {
            Application,
            Window,
            Keyboard,
            Mouse,
            Joystick,
            User
        };

        enum class EAction 
        {
            None,
            Down,
            Up,
            Repeat,
            Move
        };

        virtual Object* GetSender() const = 0;
        virtual EDevice GetDevice() const = 0;
        virtual EAction GetAction() const = 0;
    };
}