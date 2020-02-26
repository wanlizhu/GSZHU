#pragma once

#ifndef __ANDROID__
#include "Events/IEvent.h"
#include "Window/Windows/GLFW_Types.h"

namespace Wanli
{
    class WANLI_API IMouseEvent : public IEvent
    {
    public:
        friend class EventPool;
        using PTR = std::shared_ptr<IMouseEvent>;

    protected:
        IMouseEvent(const std::type_index& type, int size)
            : IEvent(type, size)
        {}
    };

    class WANLI_API MouseButtonDownEvent : public IMouseEvent
    {
    public:
        friend class EventPool;
        using PTR = std::shared_ptr<MouseButtonDownEvent>;

        EMouseButton button;
        KeyMods mods;

    protected:
        MouseButtonDownEvent(EMouseButton button, KeyMods mods)
            : IMouseEvent(typeid(MouseButtonDownEvent), sizeof(MouseButtonDownEvent))
            , button(button)
            , mods(mods)
        {}
    };

    class WANLI_API MouseButtonUpEvent : public IMouseEvent
    {
    public:
        friend class EventPool;
        using PTR = std::shared_ptr<MouseButtonUpEvent>;

        EMouseButton button;
        KeyMods mods;

    protected:
        MouseButtonUpEvent(EMouseButton button, KeyMods mods)
            : IMouseEvent(typeid(MouseButtonUpEvent), sizeof(MouseButtonUpEvent))
            , button(button)
            , mods(mods)
        {}
    };

    class WANLI_API MouseScrollEvent : public IMouseEvent
    {
    public:
        friend class EventPool;
        using PTR = std::shared_ptr<MouseScrollEvent>;

        glm::vec2 delta;

    protected:
        explicit MouseScrollEvent(const glm::vec2& delta)
            : IMouseEvent(typeid(MouseScrollEvent), sizeof(MouseScrollEvent))
            , delta(delta)
        {}
    };

    class WANLI_API MouseMoveEvent : public IMouseEvent
    {
    public:
        friend class EventPool;
        using PTR = std::shared_ptr<MouseMoveEvent>;

        glm::ivec2 pos;

    protected:
        explicit MouseMoveEvent(const glm::ivec2& pos)
            : IMouseEvent(typeid(MouseMoveEvent), sizeof(MouseMoveEvent))
            , pos(pos)
        {}
    };

    class WANLI_API MouseEnterEvent : public IMouseEvent
    {
    public:
        friend class EventPool;
        using PTR = std::shared_ptr<MouseEnterEvent>;

    protected:
        MouseEnterEvent()
            : IMouseEvent(typeid(MouseEnterEvent), sizeof(MouseEnterEvent))
        {}
    };

    class WANLI_API MouseLeaveEvent : public IMouseEvent
    {
    public:
        friend class EventPool;
        using PTR = std::shared_ptr<MouseLeaveEvent>;

    protected:
        MouseLeaveEvent()
            : IMouseEvent(typeid(MouseLeaveEvent), sizeof(MouseLeaveEvent))
        {}
    };

    class WANLI_API MouseFileDroppedEvent : public IMouseEvent
    {
    public:
        friend class EventPool;
        using PTR = std::shared_ptr<MouseFileDroppedEvent>;

        StringArray paths;

    protected:
        explicit MouseFileDroppedEvent(const StringArray& paths)
            : IMouseEvent(typeid(MouseFileDroppedEvent), sizeof(MouseFileDroppedEvent))
            , paths(paths)
        {}
    };
}
#endif