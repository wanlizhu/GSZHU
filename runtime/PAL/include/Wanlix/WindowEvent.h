#pragma once

#include "Common.h"

namespace Wanlix
{
    enum class WindowEvent
    {
        Quit,
        Draw,
        KeyDown,
        KeyUp,
        DoubleClick,
        Char,
        WheelMotion,
        LocalMotion,
        GlobalMotion,
        Resize,
        GetFocus,
        LostFocus,
    };

    class WindowEventListener
    {
        friend class Window;
    public:
        virtual void OnQuit(Window& sender) {}
        virtual void OnDraw(Window& sender) {}
        virtual void OnGetFocus(Window& sender) {}
        virtual void OnLostFocus(Window& sender) {}
        virtual void OnKeyDown(Window& sender, Key keyCode) {}
        virtual void OnKeyUp(Window& sender, Key keyCode) {}
        virtual void OnDoubleClick(Window& sender, Key keyCode) {}
        virtual void OnChar(Window& sender, wchar_t chr) {}
        virtual void OnWheelMotion(Window& sender, int motion) {}
        virtual void OnLocalMotion(Window& sender, const INT2& position) {}
        virtual void OnGlobalMotion(Window& sender, const INT2& motion) {}
        virtual void OnResize(Window& sender, const INT2& clientAreaSize) {}
    };
}