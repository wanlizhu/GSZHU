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
                return SharedPtr<T>(new T(std::forward<Args>(args)...), 
                    [&](T* ptr) {
                        ptr->~T();
                        smEventCaches[sizeof(T)].push_back(ptr);
                    });
            }
            else
            {
                IEvent* mem = static_cast<T*>(smEventCaches[sizeof(T)].front());
                smEventCaches[sizeof(T)].pop_front();
                T* ptr = new(mem) T(std::forward<Args>(args)...);
                return SharedPtr<T>(ptr);
            }
        }

        static void Clear();

    private:
        static HashMap<size_t, std::list<IEvent*>> smEventCaches;
    };
}