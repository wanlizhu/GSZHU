#pragma once

#include "Core/Common.h"

namespace Wanli
{
    class IEvent;

    class WANLI_API EventPool
    {
    public:
        template<typename T, typename... Args>
        static inline SharedPtr<T> Allocate(Args... args)
        {
            if (smEventCaches[sizeof(T)].empty())
            {
                return SharedPtr<T>(new T(std::forward<Args>(args)...));
            }

            auto& list = smEventCaches[sizeof(T)];
            auto event = std::static_pointer_cast<T>(list.front());
            list.pop_front();
            
            new(event.get()) T(std::forward<Args>(args)...);

            return event;
        }

        static void Free(SharedPtr<IEvent> event);

    private:
        static HashMap<size_t, std::list<SharedPtr<IEvent>>> smEventCaches;
    };
}