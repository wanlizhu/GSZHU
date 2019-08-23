#pragma once

#include "Function.h"
#include "Observer.h"

namespace GS
{
    template<typename RT, typename MUTEX = NoopMutex>
    class Signal;


    template<typename RT, typename MUTEX, typename... ARGS>
    class Signal<RT(ARGS...), MUTEX> final : public Observer<MUTEX>
    {
        using Observer = Observer<MUTEX>;
        using Function = Function<RT(ARGS...)>;

        template<typename T>
        void insert_sfinae(DelegateKey const& key, typename T::Observer* instance)
        {
            Observer::insert(key, instance);
            instance->insert(key, this);
        }
        template<typename T>
        void remove_sfinae(DelegateKey const& key, typename T::Observer* instance)
        {
            Observer::remove(key, instance);
            instance->remove(key, this);
        }
        template<typename T>
        void insert_sfinae(DelegateKey const& key, ...)
        {
            Observer::insert(key, this);
        }
        template<typename T>
        void remove_sfinae(DelegateKey const& key, ...)
        {
            Observer::remove(key, this);
        }

    public:
        //-------------------------------------------------------------------CONNECT
        template<typename L>
        void connect(L* instance) // connect a functor
        {
            Observer::insert(Function::template bind(instance), this);
        }
        template<typename L>
        void connect(L& instance)
        {
            connect(std::addressof(instance));
        }

        template<RT(*func_ptr)(ARGS...)>
        void connect() // connect a free function
        {
            Observer::insert(Function::template bind<func_ptr>(), this);
        }

        template<typename T, RT(T::*member_ptr)(ARGS...)>
        void connect(T* instance) // connect an overloaded member function
        {
            insert_sfinae<T>(Function::template bind<member_ptr>(instance), instance);
        }
        template<typename T, RT(T::*member_ptr)(ARGS...) const>
        void connect(T* instance) // connect an const overloaded member function 
        {
            insert_sfinae<T>(Function::template bind<member_ptr>(instance), instance);
        }

        template<typename T, RT(T::*member_ptr)(ARGS...)>
        void connect(T& instance)
        {
            connect<member_ptr, T>(std::addressof(instance));
        }
        template<typename T, RT(T::*member_ptr)(ARGS...) const>
        void connect(T& instance)
        {
            connect<member_ptr, T>(std::addressof(instance));
        }

        template<auto member_ptr, typename T>
        void connect(T* instance) // connect a member function 
        {
            insert_sfinae<T>(Function::template bind<member_ptr>(instance), instance);
        }
        template<auto member_ptr, typename T>
        void connect(T& instance)
        {
            connect<member_ptr, T>(std::addressof(instance));
        }

        //----------------------------------------------------------------DISCONNECT
        template<typename L>
        void disconnect(L* instance) // for functor
        {
            Observer::remove(Function::template bind(instance), this);
        }
        template<typename L>
        void disconnect(L& instance)
        {
            disconnect(std::addressof(instance));
        }

        template<RT(*func_ptr)(ARGS...)>
        void disconnect() // for free function
        {
            Observer::remove(Function::template bind<func_ptr>(), this);
        }

        template<typename T, RT(T::*member_ptr)(ARGS...)>
        void disconnect(T* instance) // for overloaded member function
        {
            remove_sfinae<T>(Function::template bind<member_ptr>(instance), instance);
        }
        template<typename T, RT(T::*member_ptr)(ARGS...) const>
        void disconnect(T* instance) // for const overloaded member function
        {
            remove_sfinae<T>(Function::template bind<member_ptr>(instance), instance);
        }

        template<typename T, RT(T::*member_ptr)(ARGS...)>
        void disconnect(T& instance)
        {
            disconnect<T, member_ptr>(std::addressof(instance));
        }
        template<typename T, RT(T::*member_ptr)(ARGS...) const>
        void disconnect(T& instance)
        {
            disconnect<T, member_ptr>(std::addressof(instance));
        }

        template<auto member_ptr, typename T>
        void disconnect(T* instance) // for member function
        {
            remove_sfinae<T>(Function::template bind<member_ptr>(instance), instance);
        }
        template<auto mem_ptr, typename T>
        void disconnect(T& instance)
        {
            disconnect<mem_ptr, T>(std::addressof(instance));
        }

        //----------------------------------------------------FIRE / FIRE ACCUMULATE
        template<typename... Uref>
        void fire(Uref... args) // todo: should i put it as "(Uref&&... args)"?
        {
            Observer::template for_each<Function>(std::forward<Uref>(args)...);
        }

        template<typename Accumulate, typename... Uref>
        void fire_accumulate(Accumulate&& accumulate, Uref&&... args)
        {
            Observer::template for_each_accumulate<Function, Accumulate>(
                std::forward<Accumulate>(accumulate),
                std::forward<Uref>(args)...);
        }
    };

} // namespace ZHU ------------------------------------------------------------
