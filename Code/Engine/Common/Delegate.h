#pragma once
#include "Observer.h"
#include "Invoker.h"
#include "ConstExpr.h"

namespace ZHU
{
    class ObserverVisitor
    {
    public:
        static std::weak_ptr<bool> IsValid(const Observer* observer) 
        {
            return observer->mIsValid;
        }
    };

    template<typename RETURN, typename... ARGS>
    class Delegate<RETURN(ARGS...)>
    {
    public:
        using INVOKER       = ZHU::Invoker<RETURN, ARGS...>;
        using FUNCTION      = std::function<RETURN(ARGS...)>;
        using OBSERVER_LIST = std::vector<std::weak_ptr<bool>>;
        friend class INVOKER;

        struct ObservedFunction 
        {
            FUNCTION      mFunction;
            OBSERVER_LIST mObservers;

            bool IsExpired() const {
                for (const auto& observer : mObservers) {
                    if (observer.expired())
                        return false;
                }
                return true;
            }
        };

        Delegate() = default;
        virtual ~Delegate() = default;

        template<typename... TS>
        size_t Add(FUNCTION&& func, TS... observers) 
        {
            std::lock_guard<std::mutex> lock(mMutex);
            OBSERVER_LIST observers;

            if constexpr (sizeof...(observers) != 0) {
                for (const auto& observer : { observers... }) {
                    observers.emplace_back(ObserverVisitor::IsValid(ConstExpr::AsPtr(observer)));
                }
            }

            mObservedFunctions.emplace_back(ObservedFunction{ std::move(func), observers });

            return func.target_type().hash_code();
        }

        void Remove(const FUNCTION& func)
        {
            Remove(func.target_type().hash_code());
        }

        void Remove(size_t funcHash) 
        {
            std::lock_guard<std::mutex> lock(mMutex);
            auto first = std::remove_if(mObservedFunctions.begin(), mObservedFunctions.end(), 
                                        [funcHash](ObservedFunction& obsfunc) {
                                            return funcHash == obsfunc.mFunction.target_type().hash_code();
                                        });
            mObservedFunctions.erase(first, mObservedFunctions.end());
        }

        void Clear()
        {
            std::lock_guard<std::mutex> lock(mMutex);
            mObservedFunctions.clear();
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

        Delegate& operator-=(const FUNCTION& func)
        {
            return Remove(func);
        }

        Delegate& operator-=(const size_t& funcHash)
        {
            return Remove(funcHash);
        }

    private:
        std::mutex mMutex;
        std::vector<ObservedFunction> mObservedFunctions;
    };
}