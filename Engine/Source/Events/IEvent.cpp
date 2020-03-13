#include "Events/IEvent.h"
#include "Events/EventPool.h"
#include "Events/Events.h"

namespace Wanli
{
    IEvent::IEvent(const std::type_index& type, int size)
        : mTypeIndex(type)
        , mTypeSize(size)
    {}

    IEvent::~IEvent()
    {}

    int IEvent::GetTypeSize() const
    {
        return mTypeSize;
    }

    std::type_index IEvent::GetTypeIndex() const
    {
        return mTypeIndex;
    }

    void IEvent::PostEvent()
    {
        if (Events::Get())
        {
            Events::Get()->PostEvent(shared_from_this());
        }
    }

    void IEvent::SendEvent()
    {
        if (Events::Get())
        {
            Events::Get()->SendEvent(shared_from_this());
        }
    }
}