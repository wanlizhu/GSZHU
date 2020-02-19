#pragma once

#include "Core/BasicTypes.h"

namespace Wanli
{
    template<typename>
    class Delegate;

    class DLLDECL Observer
    {
    public:
        Observer()
            : mValidFlag(std::make_shared<bool>(true))
        {}
        virtual ~Observer() = default;

        inline std::shared_ptr<bool>& GetValidFlag() { return mValidFlag; }
        inline const std::shared_ptr<bool>& GetValidFlag() const { return mValidFlag; }

    private:
        std::shared_ptr<bool> mValidFlag;
    };

    template<typename ReturnType, typename... Args>
    class Invoker
    {
    public:
        using AllReturns = std::vector<ReturnType>;

        static AllReturns Invoke(Delegate<ReturnType(Args...)>& delegate, Args... args)
        {
            std::lock_guard<std::mutex> lock(delegate.mMutex);
            AllReturns results;

            if (!delegate.mConnections.empty())
            {
                for (auto it = delegate.mConnections.begin();
                     it != delegate.mConnections.end(); 
                     it++)
                {
                    if (it->IsExpired())
                    {
                        it = delegate.mConnections.erase(it);
                        continue;
                    }
                    results.emplace_back(it->function(std::forward<Args>(args)...));
                    it++;
                }
            }

            return results;
        }
    };

    template<typename... Args>
    class Invoker<void, Args...>
    {
    public:
        using AllReturns = void;

        static void Invoke(Delegate<void(Args...)>& delegate, Args... args)
        {
            std::lock_guard<std::mutex> lock(delegate.mMutex);

            if (!delegate.mConnections.empty())
            {
                for (auto it = delegate.mConnections.begin(); 
                     it != delegate.mConnections.end();
                     it++)
                {
                    if (it->IsExpired())
                    {
                        it = delegate.mConnections.erase(it);
                        continue;
                    }
                    it->function(std::forward<Args>(args)...);
                    it++;
                }
            }
        }
    };

    template<typename ReturnType, typename... Args>
    class DLLDECL Delegate<ReturnType(Args...)>
    {
    public:
        using Invoker = Wanli::Invoker<ReturnType, Args...>;
        using Function = std::function<ReturnType(Args...)>;
        using Observers = std::vector<std::weak_ptr<bool>>;
        friend class Invoker;

        struct Connection
        {
            Function function;
            Observers observers;

            inline bool IsExpired() const
            {
                return std::any_of(observers.begin(), observers.end(), 
                                   [] (const std::weak_ptr<bool>& item) {
                                       return item.expired();
                                   });
            }
        };

        Delegate() = default;
        virtual ~Delegate() = default;

        template<typename... ObserverTypes>
        Delegate& Add(Function&& function, ObserverTypes... observers)
        {
            std::lock_guard<std::mutex> lock(mMutex);
            Observers observerList;

            if constexpr (sizeof...(observers) != 0)
            {
                for (const auto& observer : { observers... })
                {
                    observerList.emplace_back(to_address(observer)->GetValidFlag());
                }
            }

            mConnections.emplace_back(Connection{ std::move(function), observerList });;
            return *this;
        }

        Delegate& Remove(const Function& function)
        {
            std::lock_guard<std::mutex> lock(mMutex);

            mConnections.erase(std::remove_if(mConnections.begin(), mConnections.end(),
                               [&] (const Connection& conn) {
                                   return Hash(conn.function) == Hash(function);
                               }), mConnections.end());
            return *this;
        }

        template<typename... ObserverTypes>
        void RemoveObservers(ObserverTypes... observers)
        {
            Observers removes;

            if constexpr (sizeof...(observers) != 0)
            {
                for (const auto& observer : { observers... })
                {
                    removes.emplace_back(to_address(observer)->GetValidFlag());
                }
            }

            for (auto it = mConnections.begin(); it != mConnections.end();)
            {
                for (auto it1 = it->observers.begin(); it1 != it->observers.end();)
                {
                    bool erase = false;
                    auto validFlag = it1->lock();
                    for (const auto& remove : removes)
                    {
                        auto validFlag1 = remove.lock();
                        if (validFlag.get() == validFlag1.get())
                            erase = true;
                    }

                    if (erase)
                        it1 = it->observers.erase(it1);
                    else
                        it1++;
                }

                if (it->observers.empty())
                    it = mConnections.erase(it);
                else
                    it++;
            }
        }

        void MoveConnections(Delegate& from, const Observers& exclude = {})
        {
            for (auto it = from.mConnections.begin(); it < from.mConnections.end();)
            {
                bool move = true;
                for (const auto& excluded : exclude)
                {
                    auto validFlag = excluded.lock();
                    for (const auto & observer : it->observers)
                    {
                        auto validFlag1 = observer.lock();
                        if (validFlag.get() == validFlag1.get())
                            move = false;
                    }
                }

                if (move)
                {
                    std::move(from.mConnections.begin(), it, std::back_inserter(mConnections));
                    it = from.mConnections.erase(from.mConnections.begin(), it);
                }
                else
                {
                    it++;
                }
            }
        }

        void Clear()
        {
            std::lock_guard<std::mutex> lock(mMutex);
            mConnections.clear();
        }

        typename Invoker::AllReturns Invoke(Args... args)
        {
            return Invoker::Invoke(*this, std::forward<Args>(args)...);
        }

        Delegate& operator+=(Function&& function)
        {
            return Add(std::move(function));
        }

        Delegate& operator-=(const Function& function)
        {
            return Remove(function);
        }

        typename Invoker::AllReturns operator()(Args... args)
        {
            return Invoker::Invoke(*this, std::forward<Args>(args)...);
        }

    protected:
        std::mutex mMutex;
        std::vector<Connection> mConnections;
    };

    template<typename T>
    class DLLDECL DelegatedValue : public Delegate<void(T)>
        , public NonCopyable
    {
    public:
        template<typename... Args>
        DelegatedValue(Args... args)
            : mValue(std::forward<Args>(args)...)
        {}
        virtual ~DelegatedValue() = default;

        DelegatedValue& operator=(T value)
        {
            mValue = value;
            Invoke(mValue);
            return *this;
        }

        inline operator const T& () const noexcept { return mValue; }
        inline const T& Get() const { return mValue; }
        inline const T& operator*() const { return mValue; }
        inline const T* operator->() const { return &mValue; }

    protected:
        T mValue;
    };
}