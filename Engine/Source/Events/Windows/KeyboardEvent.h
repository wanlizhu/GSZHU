#pragma once

#ifdef PLATFORM_WINDOWS

#include "Events/IEvent.h"

namespace Wanli
{
    class WANLI_API KeyboardEvent : public IEvent
    {
    public:
        friend class EventPool;
        using PTR = std::shared_ptr<KeyboardEvent>;

    protected:
        KeyboardEvent(const std::type_index& type, int size)
            : IEvent(type, size)
        {}
    };

    class WANLI_API KeyDownEvent : public KeyboardEvent
    {
    public:
        friend class EventPool;
        using PTR = std::shared_ptr<KeyDownEvent>;

        EKey key;
        Optional<KeyMods> mods;

    protected:
        KeyDownEvent(EKey key)
            : KeyboardEvent(typeid(KeyDownEvent), sizeof(KeyDownEvent))
            , key(key)
            , mods(std::nullopt)
        {}
    };

    class WANLI_API KeyUpEvent : public KeyboardEvent
    {
    public:
        friend class EventPool;
        using PTR = std::shared_ptr<KeyUpEvent>;

        EKey key;
        Optional<KeyMods> mods;

    protected:
        KeyUpEvent(EKey key)
            : KeyboardEvent(typeid(KeyUpEvent), sizeof(KeyUpEvent))
            , key(key)
            , mods(std::nullopt)
        {}
    };
}

#endif