#include "Events/Event.h"
#include "Events/EventModule.h"

namespace Wanli
{
    void Event::PostEvent() const
    {
        auto module = Engine::Get()->GetModule<EventModule>();
        if (module)
        {
            auto self = std::static_pointer_cast<const IEvent>(shared_from_this());
            module->PostEvent(self);
        }
    }
}