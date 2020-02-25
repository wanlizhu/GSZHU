#pragma once

#ifdef PLATFORM_WINDOWS
#include "Events/IEvent.h"

namespace Wanli
{
    class WANLI_API MouseEvent : public IEvent
    {
    public:
        friend class EventPool;
        using PTR = std::shared_ptr<MouseEvent>;

    protected:
        MouseEvent(const std::type_index& type, int size)
            : IEvent(type, size)
        {}
    };

    class WANLI_API MouseButtonDownEvent : public MouseEvent
    {
    public:
        friend class EventPool;
        using PTR = std::shared_ptr<MouseButtonDownEvent>;

        EMouseButton button;
        Optional<KeyMods> mods;

    protected:
        MouseButtonDownEvent(EMouseButton button)
            : MouseEvent(typeid(MouseButtonDownEvent), sizeof(MouseButtonDownEvent))
            , button(button)
            , mods(std::nullopt)
        {}
    };

    class WANLI_API MouseButtonUpEvent : public MouseEvent
    {
    public:
        friend class EventPool;
        using PTR = std::shared_ptr<MouseButtonUpEvent>;

        EMouseButton button;
        Optional<KeyMods> mods;

    protected:
        MouseButtonUpEvent(EMouseButton button)
            : MouseEvent(typeid(MouseButtonUpEvent), sizeof(MouseButtonUpEvent))
            , button(button)
            , mods(std::nullopt)
        {}
    };

    class WANLI_API MouseScrollEvent : public MouseEvent
    {
    public:
        friend class EventPool;
        using PTR = std::shared_ptr<MouseScrollEvent>;

        glm::vec2 delta;
        Optional<KeyMods> mods;

    protected:
        explicit MouseScrollEvent(const glm::vec2& delta)
            : MouseEvent(typeid(MouseScrollEvent), sizeof(MouseScrollEvent))
            , delta(delta)
            , mods(std::nullopt)
        {}
    };

    class WANLI_API MouseMoveEvent : public MouseEvent
    {
    public:
        friend class EventPool;
        using PTR = std::shared_ptr<MouseMoveEvent>;

        glm::ivec2 pos;

    protected:
        explicit MouseMoveEvent(const glm::ivec2& pos)
            : MouseEvent(typeid(MouseMoveEvent), sizeof(MouseMoveEvent))
            , pos(pos)
        {}
    };

    class WANLI_API MouseEnterEvent : public MouseEvent
    {
    public:
        friend class EventPool;
        using PTR = std::shared_ptr<MouseEnterEvent>;

    protected:
        MouseEnterEvent()
            : MouseEvent(typeid(MouseEnterEvent), sizeof(MouseEnterEvent))
        {}
    };

    class WANLI_API MouseLeaveEvent : public MouseEvent
    {
    public:
        friend class EventPool;
        using PTR = std::shared_ptr<MouseLeaveEvent>;

    protected:
        MouseLeaveEvent()
            : MouseEvent(typeid(MouseLeaveEvent), sizeof(MouseLeaveEvent))
        {}
    };

    class WANLI_API MouseFileDroppedEvent : public MouseEvent
    {
    public:
        friend class EventPool;
        using PTR = std::shared_ptr<MouseFileDroppedEvent>;

        StringArray paths;

    protected:
        explicit MouseFileDroppedEvent(const StringArray& paths)
            : MouseEvent(typeid(MouseFileDroppedEvent), sizeof(MouseFileDroppedEvent))
            , paths(paths)
        {}
    };
}
#endif