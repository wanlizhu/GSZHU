#pragma once

#include "Config.h"
#include <string>
#include <vector>
#include <locale>
#include "Platform/OS.h"
#include "EnumClass.h"
#include "ConstExpr.h"
#include <optional>
#include <sstream>

namespace ZHU
{
    class ZHU_API String 
    {
    public:
        enum class EAction 
        {
            RemoveAll,
            RemoveFirst,
            RemoveLast,

            ReplaceAll,
            ReplaceFirst,
            ReplaceLast,

            TrimAll,
            TrimLeft,
            TrimRight
        };

        static std::vector<std::string> Split(std::string_view str, std::string_view sep, bool regex = false);
        static std::vector<size_t>      FindAll(std::string_view str, std::string_view token);
        static bool                     StartsWith(std::string_view str, std::string_view token);
        static bool                     Contains(std::string_view str, std::string_view token);
        static bool                     EndsWith(std::string_view str, std::string_view token);

        static bool        IsInteger(std::string_view str);
        static std::string Trim(std::string_view str, EAction action = EAction::TrimAll);
        static std::string Remove(std::string_view str, std::string_view token, EAction action = EAction::RemoveAll);
        static std::string Replace(std::string_view str, std::string_view token, std::string_view to, EAction action = EAction::ReplaceAll);
        static std::string Lowercase(std::string_view str);
        static std::string Uppercase(std::string_view str);

        template<typename T>
        static std::string From(const T& val)
        {
            if constexpr (std::is_enum_v<T>) {
                if constexpr (HasEnumFieldName<T>::value) {
                    return ENUM_TO_NAME(val);
                }
                else {
                    typedef typename std::underlying_type<T>::type safe_type;
                    return std::to_string(static_cast<safe_type>(val));
                }
            }
            else if constexpr (std::is_same_v<bool, T>) {
                return val ? "true" : "false";
            }
            else if constexpr (std::is_same_v<std::string, T>) {
                return val;
            }
            else if constexpr (std::is_same_v<std::wstring, T>) {
                return Convert<std::string>(val.c_str());
            }
            else {
                return std::to_string(val);
            }
        }

        template<typename T>
        static T To(std::string_view str) 
        {
            if constexpr (std::is_enum_v<T>)
            {
                if constexpr (HasEnumFieldName<T>::value) {
                    return NAME_TO_ENUM(T, str);
                }
                else {
                    typedef typename std::underlying_type<T>::type safe_type;
                    return static_cast<T>(To<safe_type>(str));
                }
            }
            else if constexpr (std::is_same_v<bool, T>)
            {
                return Lowercase(str) == "true" || To<std::optional<int32_t>>(str) == 1;
            }
            else if constexpr (std::is_same_v<std::string, T>)
            {
                return str;
            }
            else if constexpr (is_optional_v<T>)
            {
                typedef typename T::value_type base_type;
                base_type temp;
                std::istringstream iss(str);

                if ((iss >> temp).fail()) {
                    return std::nullopt;
                }

                return temp;
            }
            else {
                T temp;
                std::istringstream iss(str.data());
                iss >> temp;
                return temp;
            }
        }
    };
}