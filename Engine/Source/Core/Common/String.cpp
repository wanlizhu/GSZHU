#include "ENGINE_PCH.h"
#include "./String.h"
#include <regex>
#include <Engine/Core/Common/Log.h>
#include <cctype>

namespace ZHU
{
    void RegexLegalize(std::string& str)
    {
        const char* specials = "$()*+.[]?\\^{}|";
        size_t pos = str.find_first_of(specials);
        while (pos != std::string::npos) {
            str.insert(pos, "\\");
            pos = str.find_first_of(specials, pos + 2);
        }
    }

    size_t CalcNewSize(std::string_view str, std::string_view token, std::string_view to)
    {
        int diff = (int)to.size() - (int)token.size();
        int diffs = diff * (int)String::FindAll(str, token).size();
        return str.size() + diffs;
    }

    std::vector<std::string> String::Split(std::string_view str, std::string_view sep, bool regex)
    {
        using token_iterator = std::regex_token_iterator<std::string_view::const_iterator>;
        std::string regexstr(sep.data());
        if (!regex) {
            RegexLegalize(regexstr);
        }

        std::regex rx(regexstr);
        token_iterator first{ str.begin(), str.end(), rx, -1 };
        token_iterator last;

        return { first, last };
    }

    std::vector<size_t> String::FindAll(std::string_view str, std::string_view token)
    {
        std::vector<size_t> result;
        
        size_t pos = str.find(token);
        while (pos != std::string_view::npos) {
            result.push_back(pos);
            pos = str.find(token, pos + token.size());
        }

        return result;
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
        if (String::StartsWith(str, "-")) {
            if (str.size() > 1) {
                return str.find_first_not_of("0123456789", 1) == std::string_view::npos;
            }
            return false;
        }
        else {
            return str.find_first_not_of("0123456789") == std::string_view::npos;
        }
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
        std::string regex(token.data());
        RegexLegalize(regex);

        std::string result;
        result.resize(CalcNewSize(str, token, to));

        if (action == EAction::ReplaceAll) {
            std::regex_replace(result.begin(), str.begin(), str.end(), std::regex(regex), to.data());
        }
        else if (action == EAction::ReplaceFirst) {
            result = str;
            size_t pos = result.find(token);
            if (pos != std::string::npos) {
                result.replace(pos, token.size(), to);
            }
        }
        else if (action == EAction::ReplaceLast) {
            result = str;
            auto pos = result.rfind(token);
            if (pos != std::string::npos) {
                result.replace(pos, token.size(), to);
            }
        }
        else {
            SHOULDNT_HAVE_BEEN_HERE();
        }
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