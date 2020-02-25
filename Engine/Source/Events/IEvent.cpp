#include "Events/IEvent.h"
#include "Events/EventPool.h"
#include "Events/EventManager.h"

namespace Wanli
{
    IEvent::~IEvent()
    {
        EventPool::Free(shared_from_this());
    }

    std::type_index IEvent::GetTypeIndex() const
    {
        return mTypeIndex;
    }

    int IEvent::GetTypeSize() const
    {
        return mTypeSize;
    }

    void IEvent::PostEvent()
    {
        EventManager::Get()->PostEvent(shared_from_this());
    }

    void IEvent::SendEvent()
    {
        EventManager::Get()->SendEvent(shared_from_this());
    }
}