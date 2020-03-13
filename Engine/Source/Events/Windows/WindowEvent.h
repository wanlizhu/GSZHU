#pragma once

#ifndef __ANDROID__
#include "Window/Windows/GLFW_Types.h"
#include "Events/IEvent.h"
#include "Window/Windows/Menu.h"

namespace Wanli
{
    class WANLI_API IWindowEvent : public IEvent
    {
    public:
        friend class EventPool;
        using PTR = std::shared_ptr<IWindowEvent>;

    protected:
        IWindowEvent(const std::type_index& type, int size)
            : IEvent(type, size)
        {}
    };

    class WANLI_API WindowCloseEvent : public IWindowEvent
    {
    public:
        friend class EventPool;
        using PTR = std::shared_ptr<WindowCloseEvent>;

    protected:
        WindowCloseEvent()
            : IWindowEvent(typeid(WindowCloseEvent), sizeof(WindowCloseEvent))
        {}
    };

    class WANLI_API WindowIconifiedEvent : public IWindowEvent
    {
    public:
        friend class EventPool;
        using PTR = std::shared_ptr<WindowIconifiedEvent>;

    protected:
        WindowIconifiedEvent()
            : IWindowEvent(typeid(WindowIconifiedEvent), sizeof(WindowIconifiedEvent))
        {}
    };

    class WANLI_API WindowPosEvent : public IWindowEvent
    {
    public:
        friend class EventPool;
        using PTR = std::shared_ptr<WindowPosEvent>;

        glm::ivec2 pos;

    protected:
        explicit WindowPosEvent(const glm::ivec2& pos)
            : IWindowEvent(typeid(WindowPosEvent), sizeof(WindowPosEvent))
            , pos(pos)
        {}
    };

    class WANLI_API WindowResizeEvent : public IWindowEvent
    {
    public:
        friend class EventPool;
        using PTR = std::shared_ptr<WindowResizeEvent>;

        glm::ivec2 size;

    protected:
        explicit WindowResizeEvent(const glm::ivec2& size)
            : IWindowEvent(typeid(WindowResizeEvent), sizeof(WindowResizeEvent))
            , size(size)
        {}
    };

    class WANLI_API WindowRedrawEvent : public IWindowEvent
    {
    public:
        friend class EventPool;
        using PTR = std::shared_ptr<WindowRedrawEvent>;

    protected:
        WindowRedrawEvent()
            : IWindowEvent(typeid(WindowRedrawEvent), sizeof(WindowRedrawEvent))
        {}
    };

    class WANLI_API WindowMenuEvent : public IWindowEvent
    {
    public:
        friend class EventPool;
        using PTR = std::shared_ptr<WindowMenuEvent>;

        EMenuID menuId;

    protected:
        explicit WindowMenuEvent(EMenuID id)
            : IWindowEvent(typeid(WindowMenuEvent), sizeof(WindowMenuEvent))
            , menuId(id)
        {}
    };
}

#endif