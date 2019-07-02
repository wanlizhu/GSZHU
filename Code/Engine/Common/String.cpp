#include "ZHUENGINE_PCH.h"
#include "./String.h"
#include <regex>

namespace ZHU
{
    std::vector<std::string> String::Split(std::string_view str, std::string_view regex)
    {
        using token_iterator = std::regex_token_iterator<std::string_view::const_iterator>;

        std::regex rx(regex.data());
        token_iterator first{ str.begin(), str.end(), rx, -1 };
        token_iterator last;

        return { first, last };
    }

    bool String::StartsWith(std::string_view str, std::string_view token)
    {
        return str.find(token) == 0;
    }

    bool String::Contains(std::string_view str, std::string_view token)
    {
        const size_t pos = str.find(token);
        return pos != std::string_view::npos;
    }

    bool String::EndsWith(std::string_view str, std::string_view token)
    {
        if (token.size() > str.size())
            return false;

        return std::equal(token.rbegin(), token.rend(), str.rbegin());
    }
    
    bool String::IsInteger(std::string_view str)
    {
        return str.find_first_not_of("0123456789") == std::string_view::npos;
    }

    std::string String::Trim(std::string_view str, EAction action)
    {
        std::string result(str);
        if (action == EAction::TrimAll || action == EAction::TrimLeft) {
            result.erase(result.begin(), std::find_if(result.begin(), result.end(), [](int ch){
                return !std::isspace(ch);
            }));
        }
        if (action == EAction::TrimAll || action == EAction::TrimRight) {
            result.erase(std::find_if(result.rbegin(), result.rend(), [](int ch) {
                return !std::isspace(ch);
            }).base(), result.end());
        }
        return result;
    }

    std::string String::Remove(std::string_view str, std::string_view token, EAction action)
    {
        return Replace(str, token, "", action == EAction::RemoveFirst ? EAction::ReplaceFirst
                                    : (action == EAction::RemoveLast  ? EAction::ReplaceLast : EAction::ReplaceAll));
    }

    std::string String::Replace(std::string_view str, std::string_view token, std::string_view to, EAction action)
    {
        std::string result;
        std::regex_replace(result.begin(), str.begin(), str.end(), std::regex(token.data()), to.data());
        return result;
    }

    std::string String::Lowercase(std::string_view str)
    {
        std::string result;
        std::transform(str.begin(), str.end(), result.begin(), tolower);
        return result;
    }

    std::string String::Uppercase(std::string_view str)
    {
        std::string result;
        std::transform(str.begin(), str.end(), result.begin(), toupper);
        return result;
    }
}