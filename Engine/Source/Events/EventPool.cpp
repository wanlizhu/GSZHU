#include "Events/EventPool.h"
#include "Events/IEvent.h"

namespace Wanli
{
    HashMap<size_t, std::list<IEvent*>> EventPool::smEventCaches;

    void EventPool::Clear()
    {
        smEventCaches.clear();
    }
}