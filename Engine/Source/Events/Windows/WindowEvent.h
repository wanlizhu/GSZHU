#pragma once

#ifdef PLATFORM_WINDOWS

#include "Events/IEvent.h"
#include "Window/Windows/Menu.h"

namespace Wanli
{
    class WANLI_API WindowEvent : public IEvent
    {
    public:
        friend class EventPool;
        using PTR = std::shared_ptr<WindowEvent>;

    protected:
        WindowEvent(const std::type_index& type, int size)
            : IEvent(type, size)
        {}
    };

    class WANLI_API WindowCloseEvent : public WindowEvent
    {
    public:
        friend class EventPool;
        using PTR = std::shared_ptr<WindowCloseEvent>;

    protected:
        WindowCloseEvent()
            : WindowEvent(typeid(WindowCloseEvent), sizeof(WindowCloseEvent))
        {}
    };

    class WANLI_API WindowIconifiedEvent : public WindowEvent
    {
    public:
        friend class EventPool;
        using PTR = std::shared_ptr<WindowIconifiedEvent>;

    protected:
        WindowIconifiedEvent()
            : WindowEvent(typeid(WindowIconifiedEvent), sizeof(WindowIconifiedEvent))
        {}
    };

    class WANLI_API WindowPosEvent : public WindowEvent
    {
    public:
        friend class EventPool;
        using PTR = std::shared_ptr<WindowPosEvent>;

        glm::ivec2 pos;

    protected:
        explicit WindowPosEvent(const glm::ivec2& pos)
            : WindowEvent(typeid(WindowPosEvent), sizeof(WindowPosEvent))
            , pos(pos)
        {}
    };

    class WANLI_API WindowResizeEvent : public WindowEvent
    {
    public:
        friend class EventPool;
        using PTR = std::shared_ptr<WindowResizeEvent>;

        glm::ivec2 size;

    protected:
        explicit WindowResizeEvent(const glm::ivec2& size)
            : WindowEvent(typeid(WindowResizeEvent), sizeof(WindowResizeEvent))
            , size(size)
        {}
    };

    class WANLI_API WindowRedrawEvent : public WindowEvent
    {
    public:
        friend class EventPool;
        using PTR = std::shared_ptr<WindowRedrawEvent>;

    protected:
        WindowRedrawEvent()
            : WindowEvent(typeid(WindowRedrawEvent), sizeof(WindowRedrawEvent))
        {}
    };

    class WANLI_API WindowMenuEvent : public WindowEvent
    {
    public:
        friend class EventPool;
        using PTR = std::shared_ptr<WindowMenuEvent>;

        EMenuID menuId;

    protected:
        explicit WindowMenuEvent(EMenuID id)
            : WindowEvent(typeid(WindowMenuEvent), sizeof(WindowMenuEvent))
            , menuId(id)
        {}
    };
}

#endif