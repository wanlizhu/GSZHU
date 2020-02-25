#include "Events/EventPool.h"
#include "Events/IEvent.h"

namespace Wanli
{
    HashMap<size_t, std::list<SharedPtr<IEvent>>> EventPool::smEventCaches;

    void EventPool::Free(SharedPtr<IEvent> event)
    {
        smEventCaches[event->GetTypeSize()].emplace_back(event);
    }
}