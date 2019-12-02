#pragma once

#include "Wanlix/Core3D/Graphics/RHI/Surface.h"
#include "Wanlix/Core3D/Types.h"
#include "MapKey.h"

namespace Wanlix
{
    enum class Event
    {
        Undefined,
        Redraw,
        Quit,
        KeyDown,
        KeyUp,
        DoubleClick,
        Char,
        WheelMotion,
        LocalMotion,
        GlobalMotion,
        Resize,
    };

    template<SurfaceType _Type_>
    class EventListener
    {};

    class Window;

    template<>
    class EventListener<SurfaceType::DesktopWindow>
    {
    public:
        using Ptr = std::shared_ptr<EventListener>;
        using ConstPtr = std::shared_ptr<const EventListener>;

        virtual void OnRedraw(Window& sender) {}
        virtual void OnQuit(Window& sender) {}
        virtual void OnKeyDown(Window& sender, Key keyCode) {}
        virtual void OnKeyUp(Window& sender, Key keyCode) {}
        virtual void OnDoubleClick(Window& sender, Key button) {}
        virtual void OnChar(Window& sender, wchar_t chr) {}
        virtual void OnWheelMotion(Window& sender, int motion) {}
        virtual void OnLocalMotion(Window& sender, const Offset& pos) {}
        virtual void OnGlobalMotion(Window& sender, const Offset& motion) {}
        virtual void OnResize(Window& sender, const Extent& extent) {}
    };

    class Canvas;

    template<>
    class EventListener<SurfaceType::MobileCanvas>
    {
    public:
        using Ptr = std::shared_ptr<EventListener>;
        using ConstPtr = std::shared_ptr<const EventListener>;

        virtual void OnRedraw(Canvas& sender) {}
        virtual void OnQuit(Canvas& sender) {}
        virtual void OnTouch(Canvas& sender, const Offset& pos) {}
        virtual void OnTouchMotion(Canvas& sender, const Offset& pos) {}
        virtual void OnLongTouch(Canvas& sender, const Offset& pos) {}
    };
}