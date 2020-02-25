#include "Events/EventManager.h"

namespace Wanli
{
    void EventManager::Initialize()
    {}
    
    void EventManager::Update()
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

    void EventManager::Destroy()
    {}

    void EventManager::PostEvent(const IEvent::PTR& event)
    {
        mEventQueue.emplace(event);
    }

    void EventManager::SendEvent(const IEvent::PTR& event)
    {
        mEventQueue.emplace(event);
    }
}