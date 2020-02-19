#include "Events/EventModule.h"
#include "Utilities/Log.h"

namespace Wanli
{
    EventModule::~EventModule()
    {}

    void EventModule::Initialize(IConfig* config)
    {}

    void EventModule::Update(double elapsedTime)
    {}

    void EventModule::Shutdown()
    {}

    void EventModule::RemoveEventCallbacks(EEventType type)
    {
        auto it = mEventCallbacks.find(type);
        if (it != mEventCallbacks.end())
        {
            mEventCallbacks.erase(it);
        }
    }

    void EventModule::PostEvent(SharedPtr<const IEvent> event) const
    {
        if (mEventCallbacks.find(event->GetEventType()) == mEventCallbacks.end())
        {
            return;
        }

        for (const auto& callback : mEventCallbacks.at(event->GetEventType()))
        {
            callback(event->GetData());
        }
    }
}