#pragma once

#include <cstdint>
#include <array>

namespace ZHU
{
    // a pair of instance pointer and function pointer
    using DelegateKey = std::array<const std::uintptr_t, 2>; // two uint64_t
    template<typename RT> class Function;                    // |
                                                             // |
                                                             // |
    template<typename RT, typename... ARGS>                  // |
    class Function<RT(ARGS...)> final // two pointers <---------+
    {
        // Only ZHU::Observer is allowed access
        template<typename> friend class Observer;
        using Thunk = RT(*)(void*, ARGS&&...);

        static inline Function bind(DelegateKey const& delegate_key)
        {
            return // reinterpret two uint64_t as two pointers
            {
                reinterpret_cast<void*>(delegate_key[0]), // instance pointer
                reinterpret_cast<Thunk>(delegate_key[1]) // function pointer
            };
        }

    public:
        template<auto func_ptr>
        static inline Function bind() // bind a free function
        {
            return
            {
                nullptr,
                [] (void*, ARGS&&... args) {
                    return (*func_ptr)(std::forward<ARGS>(args)...);
                }
            };
        }

        template<auto member_ptr, typename T>
        static inline Function bind(T* pointer) // bind a member function of class
        {
            return
            {
                pointer, 
                [] (void* this_ptr, ARGS&&... args) {
                    return (static_cast<T*>(this_ptr)->*member_ptr)(std::forward<ARGS>(args)...);
                }
            };
        }

        template<typename L>
        static inline Function bind(L* pointer) // bind a functor
        {
            return
            {
                pointer, 
                [] (void* this_ptr, ARGS&&... args) {
                    return static_cast<L*>(this_ptr)->operator()(std::forward<ARGS>(args)...);
                }
            };
        }

        template<typename... Uref>
        inline RT operator() (Uref&&... args) // make a function call
        {
            return (*m_function_pointer)(m_instance_pointer, std::forward<Uref>(args)...);
        }

        inline operator DelegateKey() const
        {
            return
            {
                reinterpret_cast<std::uintptr_t>(m_instance_pointer),
                reinterpret_cast<std::uintptr_t>(m_function_pointer)
            };
        }

    public:
        void* m_instance_pointer;
        const Thunk m_function_pointer;
    };
} // namespace ZHU ------------------------------------------------------------
