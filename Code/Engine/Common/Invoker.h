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

            for (auto it = delegate.mObservedFunctions.begin(); it != delegate.mObservedFunctions.end();) {
                if (it->IsExpired()) {
                    it = delegate.mObservedFunctions.erase(it);
                    continue;
                }

                returns.emplace_back((it->mFunction)(std::forward<ARGS>(args)...));
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
            
            for (auto it = delegate.mObservedFunctions.begin(); it != delegate.mObservedFunctions.end();) {
                if (it->IsExpired()) {
                    it = delegate.mObservedFunctions.erase(it);
                    continue;
                }

                it->mFunction(std::forward<ARGS>(args)...);
                ++it;
            }
        }
    };
}