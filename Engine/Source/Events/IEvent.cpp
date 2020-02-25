#include "Events/IEvent.h"
#include "Events/EventPool.h"
#include "Events/EventManager.h"

namespace Wanli
{
    IEvent::~IEvent()
    {
        EventPool::Free(shared_from_this());
    }

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
        EventManager::Get()->PostEvent(shared_from_this());
    }

    void IEvent::SendEvent()
    {
        EventManager::Get()->SendEvent(shared_from_this());
    }
}