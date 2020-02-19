#pragma once

#include "Events/IEvent.h"

namespace Wanli
{
    class DLLDECL Event : public IEvent
    {
    public:
        template<typename... Args>
        static SharedPtr<Event> Create(EEventType eventType, Args... args)
        {
            auto ptr = new Event(eventType, std::forward<Args>(args)...);
            return SharedPtr<Event>(ptr);
        }

        virtual Uint GetID() const override final { return mId; }
        virtual EEventType GetEventType() const override final { return mEventType; }
        virtual const std::any& GetData() const override final { return mArgs; }

        void PostEvent() const;

    protected:
        Event(EEventType eventType)
            : mId(UniqueID<IEvent>::Get())
            , mEventType(eventType)
            , mArgs{}
        {}

        template<typename... Args>
        explicit Event(EEventType eventType, Args... args)
            : mId(UniqueID<IEvent>::Get())
            , mEventType(eventType)
            , mArgs(std::tuple(std::forward<Args>(args)...))
        {}

    protected:
        Uint mId;
        EEventType mEventType;
        std::any mArgs;
    };
}