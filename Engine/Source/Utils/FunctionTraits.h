#pragma once

#include <functional>
#include <type_traits>
#include <tuple>

namespace Wanli
{
    template<typename T>
    struct FunctionTraits : public FunctionTraits<decltype(&T::operator())>
    {};

    template<typename ClassType,
        typename ReturnType,
        typename... Args>
        struct FunctionTraits<ReturnType(ClassType::*)(Args...) const>
    {
        enum { arity = sizeof...(Args); };
        using return_type = ReturnType;

        template<int i>
        struct Arg
        {
            using type = typename std::tuple_element<i, std::tuple<Args...>>::type;
        };
    };

    template<typename T>
    constexpr typename std::underlying_type<T>::type
        EnumCast(T x)
    {
        using final_type = typename std::underlying_type<T>::type;
        return static_cast<final_type>(x);
    }

    template<typename To, typename From>
    constexpr std::unique_ptr<To> 
        UniquePtrCast(std::unique_ptr<From>& from) noexcept
    {
        if constexpr (std::is_base_of_v<To, From>)
        {
            // Up cast
            return std::unique_ptr<To>(static_cast<To*>(from.release()));
        }
        else if constexpr (std::is_base_of_v<From, To>)
        {
            // Down cast
            To* to = dynamic_cast<To>(from.get());
            if (to != nullptr)
            {
                from.release();
            }
            return std::unique_ptr<To>(to);
        }
        else
        {
            static_assert(false);
        }
    }
}