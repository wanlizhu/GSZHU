#pragma once

#ifdef PLATFORM_WINDOWS
#include "Events/IEvent.h"
#include "Window/Windows/GLFW_Types.h"

namespace Wanli
{
    class WANLI_API IKeyboardEvent : public IEvent
    {
    public:
        friend class EventPool;
        using PTR = std::shared_ptr<IKeyboardEvent>;

    protected:
        IKeyboardEvent(const std::type_index& type, int size)
            : IEvent(type, size)
        {}
    };

    class WANLI_API KeyDownEvent : public IKeyboardEvent
    {
    public:
        friend class EventPool;
        using PTR = std::shared_ptr<KeyDownEvent>;

        EKey key;
        KeyMods mods;

    protected:
        KeyDownEvent(EKey key, KeyMods mods)
            : IKeyboardEvent(typeid(KeyDownEvent), sizeof(KeyDownEvent))
            , key(key)
            , mods(mods)
        {}
    };

    class WANLI_API KeyUpEvent : public IKeyboardEvent
    {
    public:
        friend class EventPool;
        using PTR = std::shared_ptr<KeyUpEvent>;

        EKey key;
        KeyMods mods;

    protected:
        KeyUpEvent(EKey key, KeyMods mods)
            : IKeyboardEvent(typeid(KeyUpEvent), sizeof(KeyUpEvent))
            , key(key)
            , mods(mods)
        {}
    };
}

#endif