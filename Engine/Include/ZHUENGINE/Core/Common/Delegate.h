#pragma once
#include <ZHUENGINE/Core/Common/IObserver.h>
#include <ZHUENGINE/Core/Common/Invoker.h>
#include <ZHUENGINE/Core/Common/ConstExpr.h>

namespace ZHU
{
    template<typename RETURN, typename... ARGS>
    class Delegate<RETURN(ARGS...)>
    {
    public:
        using INVOKER  = Invoker<RETURN, ARGS...>;
        using FUNCTION = std::function<RETURN(ARGS...)>;
        using OBSERVER = std::weak_ptr<IObserver<ARGS...>>;
        friend class INVOKER;

        Delegate() = default;
        virtual ~Delegate() = default;

        size_t Add(FUNCTION&& func) 
        {
            std::lock_guard<std::mutex> lock(mMutex);
            mFunctions.emplace_back(std::move(func));

            return func.target_type().hash_code();
        }

        size_t Add(const OBSERVER& observer)
        {
            std::lock_guard<std::mutex> lock(mMutex);
            auto realptr = observer.lock();
            if (realptr) {
                mObservers.emplace_back(observer);
                return std::hash()(realptr.get());
            }
            return 0;
        }

        void Remove(size_t hash) 
        {
            std::lock_guard<std::mutex> lock(mMutex);
            auto first = std::remove_if(mFunctions.begin(), mFunctions.end(), 
                                        [hash](FUNCTION& func) {
                                            return hash == func.target_type().hash_code();
                                        });
            mFunctions.erase(first, mFunctions.end());

            if (first == mFunctions.end()) {
                auto first = std::remove_if(mObservers.begin(), mObservers.end(), 
                                            [hash](OBSERVER& observer) {
                                               auto realptr = observer.lock();
                                               return hash == std::hash()(realptr.get());
                                            });
                mObservers.erase(first, mObservers.end());
            }
        }

        void Clear()
        {
            std::lock_guard<std::mutex> lock(mMutex);
            mFunctions.clear();
            mObservers.clear();
        }

        typename INVOKER::RETURN_VEC Invoke(ARGS... args)
        {
            return INVOKER::Invoke(*this, std::forward<ARGS>(args)...);
        }

        typename INVOKER::RETURN_VEC operator()(ARGS... args) 
        {
            return INVOKER::Invoke(*this, std::forward<ARGS>(args)...);
        }

        Delegate& operator+=(FUNCTION&& func)
        {
            return Add(std::move(func));
        }

        Delegate& operator+=(const OBSERVER& observer)
        {
            return Add(observer);
        }

        Delegate& operator-=(const FUNCTION& func)
        {
            return Remove(func.target_type().hash_code());
        }

        Delegate& operator-=(const OBSERVER& observer)
        {
            auto realptr = observer.lock();
            return Remove(std::hash()(realptr.get()));
        }

    private:
        std::mutex            mMutex;
        std::vector<FUNCTION> mFunctions;
        std::vector<OBSERVER> mObservers;
    };
}