#pragma once
#include <ZHUENGINE/Config.h>
#include <ZHUENGINE/Core/Common/BasicTypes.h>
#include <string_view>
#include <assert.h>
#include <stdexcept>
#include <type_traits>
#include <unordered_map>
#include <algorithm>

namespace ZHU
{
    template<typename T>
    struct HasEnumFieldName : std::false_type {};

    template<typename T>
    class EnumFieldName 
    {};

    template<typename T>
    struct EnableBitOperators : std::false_type {};

    template<typename T>
    struct Enumerator 
    {
        using underlying_type = typename std::underlying_type<T>::type;
        underlying_type value;

        constexpr Enumerator(const T& val) : value(static_cast<underlying_type>(val)) {}
        constexpr Enumerator(const underlying_type& val) : value(val) {}
        constexpr operator bool() const { return value != 0; }
        constexpr operator T() const { return static_cast<T>(value); }
        constexpr operator underlying_type() const { return value; }
    };

    // operator&(T, T)
    template<typename T>
    constexpr typename std::enable_if_t<std::is_enum_v<T> && EnableBitOperators<T>::value,
                                        typename std::underlying_type<T>::type> 
    operator&(const Enumerator<T>& lhs, const Enumerator<T>& rhs)
    {
        using U = typename std::underlying_type<T>::type;
        return static_cast<U>(lhs) & static_cast<U>(rhs);
    }

    // operator|(T, T)
    template<typename T>
    constexpr typename std::enable_if_t<std::is_enum_v<T> && EnableBitOperators<T>::value,
                                        typename std::underlying_type<T>::type>
    operator|(const Enumerator<T>& lhs, const Enumerator<T>& rhs)
    {
        using U = typename std::underlying_type<T>::type;
        return static_cast<U>(lhs) | static_cast<U>(rhs);
    }

    // operator^(T, T)
    template<typename T>
    constexpr typename std::enable_if_t<std::is_enum_v<T> && EnableBitOperators<T>::value,
                                        typename std::underlying_type<T>::type>
    operator^(const Enumerator<T>& lhs, const Enumerator<T>& rhs)
    {
        using U = typename std::underlying_type<T>::type;
        return static_cast<U>(lhs) ^ static_cast<U>(rhs);
    }

    // operator~(T)
    template<typename T>
    constexpr typename std::enable_if_t<std::is_enum_v<T>&& EnableBitOperators<T>::value,
                                        typename std::underlying_type<T>::type>
    operator~(const Enumerator<T>& rhs)
    {
        using U = typename std::underlying_type<T>::type;
        return ~static_cast<U>(rhs);
    }
}

#define ENABLE_BIT_OPERATORS(Enum) template<> struct EnableBitOperators<Enum> : std::true_type {};

#define BEGIN_ENUM_FIELDS(Enum)\
    template<> struct ZHU::HasEnumFieldName<Enum> : std::true_type{};\
    template<>\
    class ZHU::EnumFieldName<Enum> {\
        using safe_type = typename std::underlying_type<Enum>::type;\
        using hash_map = std::unordered_map<safe_type, std::string_view>;\
        using pair = std::pair<safe_type, std::string_view>;\
    public:\
        static std::string_view GetNameByEnum(const Enum& e) { return smFieldNames.at(static_cast<safe_type>(e)); }\
        static Enum             GetEnumByName(std::string_view name) {\
            auto it = std::find_if(smFieldNames.begin(), smFieldNames.end(), [&](const pair& it){\
                return it.second == name;\
            });\
            assert(it != smFieldNames.end());\
            return static_cast<Enum>(it->first);\
        }\
    \
    private:\
        static const hash_map smFieldNames;\
    };\
    \
    const ZHU::EnumFieldName<Enum>::hash_map    ZHU::EnumFieldName<Enum>::smFieldNames = {

#define ADD_ENUM_FIELD2(Field, Name) { static_cast<std::underlying_type<decltype(Field)>::type>(Field), std::string_view(Name) }
#define ADD_ENUM_FIELD(Field)        ADD_ENUM_FIELD2(Field, #Field)
#define END_ENUM_FIELDS              };

#define ENUM_TO_NAME(e)          ZHU::EnumFieldName<std::remove_reference_t<std::remove_cv_t<decltype(e)>>>::GetNameByEnum(e)
#define NAME_TO_ENUM(Enum, name) ZHU::EnumFieldName<Enum>::GetEnumByName(name)