#include "Events/Events.h"
#include "Utils/Log.h"

namespace Wanli
{
    Events::Events()
    {}

    Events::~Events()
    {
        LOG_DEBUG("[IModule] Events Destroy\n");
    }

    void Events::Update()
    {
        while (!mEventQueue.empty())
        {
            const auto& typeIndex = mEventQueue.front()->GetTypeIndex();
            for (const auto& callback : mCallbackMap[typeIndex])
            {
                callback(mEventQueue.front());
            }
            mEventQueue.pop();
        }
    }

    void Events::PostEvent(const IEvent::PTR& event)
    {
        mEventQueue.push(event);
    }

    void Events::SendEvent(const IEvent::PTR& event)
    {
        mEventQueue.push(event);
    }
}