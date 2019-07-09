#pragma once
#include <vector>
#include <mutex>
#include <thread>

namespace ZHU
{
    template<typename>
    class Delegate;

    template<typename RETURN, typename... ARGS>
    class Invoker
    {
    public:
        using RETURN_VEC = std::vector<RETURN>;

        static RETURN_VEC Invoke(Delegate<RETURN(ARGS...)>& delegate, ARGS... args)
        {
            std::lock_guard<std::mutex> lock(delegate.mMutex);
            RETURN_VEC returns;

            for (auto it = delegate.mFunctions.begin(); it != delegate.mFunctions.end();) {
                returns.emplace_back((*it)(std::forward<ARGS>(args)...));
                ++it;
            }

            for (auto it = delegate.mObservers.begin(); it != delegate.mObservers.end();) {
                auto realptr = (*it).lock();
                if (realptr == nullptr) {
                    it = delegate.mObservers.erase(it);
                    continue;
                }

                returns.emplace_back(realptr->Update(std::forward<ARGS>(args)...));
                ++it;
            }

            return returns;
        }
    };


    template<typename... ARGS>
    class Invoker<void, ARGS...>
    {
    public:
        using RETURN_VEC = void;

        static void Invoke(Delegate<void(ARGS...)>& delegate, ARGS... args)
        {
            std::lock_guard<std::mutex> lock(delegate.mMutex);
            
            for (auto it = delegate.mFunctions.begin(); it != delegate.mFunctions.end();) {
                (*it)(std::forward<ARGS>(args)...);
                ++it;
            }

            for (auto it = delegate.mObservers.begin(); it != delegate.mObservers.end();) {
                auto realptr = (*it).lock();
                if (realptr == nullptr) {
                    it = delegate.mObservers.erase(it);
                    continue;
                }

                realptr->Update(std::forward<ARGS>(args)...);
                ++it;
            }
        }
    };
}