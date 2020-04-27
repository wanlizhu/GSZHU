#pragma once

#include <string>
#include <string_view>
#include <vector>
#include <algorithm>
#include <functional>
#include <cstdint>
#include <cassert>
#include "dejavu/core/preprocess.h"

namespace djv
{
    template<typename charT>
    struct ichar_traits : public std::char_traits<charT>
    {
        inline static bool eq(charT c1, charT c2)
        {
            return std::tolower(c1) == std::tolower(c2);
        }

        inline static bool ne(charT c1, charT c2)
        {
            return !eq(c1, c2);
        }

        inline static bool lt(charT c1, charT c2)
        {
            return std::tolower(c1) < std::tolower(c2);
        }

        inline static int compare(const charT* s1, const charT* s2, std::size_t n)
        {
            for (std::size_t i = 0; i < n && s1 && s2; ++i, ++s1, ++s2)
            {
                if (std::tolower(*s1) == std::tolower(*s2))
                    continue;
                else if (std::tolower(*s1) < std::tolower(*s2))
                    return -1;
                else
                    return 1;
            }
            return 0;
        }

        inline static const charT* find(const charT* s, std::size_t n, charT a)
        {
            for (std::size_t i = 0; i < n; i++)
            {
                if (std::tolower(*(s + i)) == std::tolower(a))
                    return s + i;
            }

            return nullptr;
        }
    };

    /*
     * On Windows, wchar_t is UTF-16 while on other platform such as Linux and MacOS, wchar_t is UTF-32.
     * So, these two functions are platform specific code.
    */
    std::string DJV_API to_string(const std::wstring& wstr);
    std::wstring DJV_API to_wstring(const std::string& str);

    template<typename charT>
    inline constexpr bool is_ascii_space(const charT& ch)
    {
        if constexpr (std::is_same_v<std::decay_t<charT>, char>)
        {
            return std::strchr(" \t\r\n", ch) != nullptr;
        }
        else if constexpr (std::is_same_v<std::decay_t<charT>, wchar_t>)
        {
            return std::wcschr(L" \t\r\n", ch) != nullptr;
        }
        else
        {
            return false;
        }
    }

    template<typename charT>
    inline constexpr bool is_ascii_alphabet(const charT& ch)
    {
        if constexpr (std::is_same_v<std::decay_t<charT>, char>)
        {
            return std::strchr("ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz", ch) != nullptr;
        }
        else if constexpr (std::is_same_v<std::decay_t<charT>, wchar_t>)
        {
            return std::wcschr(L"ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz", ch) != nullptr;
        }
        else 
        {
            return false;
        }
    }

    template<typename charT>
    inline constexpr bool is_ascii_digit(const charT& ch)
    {
        if constexpr (std::is_same_v<std::decay_t<charT>, char>)
        {
            return std::strchr("0123456789", ch) != nullptr;
        }
        else if constexpr (std::is_same_v<std::decay_t<charT>, wchar_t>)
        {
            return std::wcschr(L"0123456789", ch) != nullptr;
        }
        else 
        {
            return false;
        }
    }

    template<typename charT, typename charTrait>
    inline constexpr std::basic_string<charT>& trim_inplace(std::basic_string<charT, charTrait>& str)
    {
        using T = typename std::basic_string<charT, charTrait>;
        typename T::size_type first = 0;
        typename T::size_type last = str.size() - 1;

        while (first <= last && is_ascii_space(str[first])) {
            ++first;
        }
        while (last >= first && is_ascii_space(str[last])) {
            --last;
        }

        if (last >= 0) {
            str.resize(last + 1);
            str.erase(0, first);
        }

        return str;
    }

    template<typename charT, typename charTrait>
    inline constexpr std::basic_string<charT>& toupper_inplace(std::basic_string<charT, charTrait>& str)
    {
        using T = typename std::basic_string<charT, charTrait>;
        auto it = str.begin();
        auto end = str.end();

        while (it != end)
        {
            *it = static_cast<typename T::value_type>(std::toupper(*it));
            ++it;
        }

        return str;
    }

    template<typename charT, typename charTrait>
    inline constexpr std::basic_string<charT>& tolower_inplace(std::basic_string<charT, charTrait>& str)
    {
        using T = typename std::basic_string<charT, charTrait>;
        auto it = str.begin();
        auto end = str.end();

        while (it != end) 
        {
            *it = static_cast<typename T::value_type>(std::tolower(*it));
            ++it;
        }

        return str;
    }

    template<typename charT, typename charTrait>
    inline constexpr bool starts_with(
        const std::basic_string<charT, charTrait>& str, 
        const std::basic_string<charT, charTrait>& prefix
    )
    {
        //using T = typename std::basic_string<charT, charTrait>;

        if (str.size() >= prefix.size())
        {
            auto it = str.find(prefix);
            return it == 0;
        }

        return false;
    }

    template<typename charT, typename charTrait, typename U, std::enable_if_t<std::is_constructible_v<std::basic_string<charT, charTrait>, const U*>, int> = 0>
    inline constexpr bool starts_with(
            const std::basic_string<charT, charTrait>& str,
            const U* prefix
        )
    {
        return starts_with<charT, charTrait>(str, std::basic_string<charT, charTrait>(prefix));
    }

    template<typename charT, typename charTrait>
    inline constexpr bool ends_with(
        const std::basic_string<charT, charTrait>& str,
        const std::basic_string<charT, charTrait>& suffix
    )
    {
        //using T = typename std::basic_string<charT, charTrait>;
        
        if (str.size() >= suffix.size())
        {
            auto it = str.rfind(suffix);
            return it == str.end() - suffix.size() - str.begin();
        }

        return false;
    }

    template<typename charT, typename charTrait, typename U, std::enable_if_t<std::is_constructible_v<std::basic_string<charT, charTrait>, const U*>, int> = 0>
    inline constexpr bool ends_with(
        const std::basic_string<charT, charTrait>& str,
        const U* suffix
    )
    {
        return ends_with<charT, charTrait>(str, std::basic_string<charT, charTrait>(suffix));
    }

    template<typename charT, typename charTrait>
    inline constexpr bool starts_of(
        const std::basic_string<charT, charTrait>& str,
        const std::basic_string<charT, charTrait>& prefixes
    )
    {
        //using T = typename std::basic_string<charT, charTrait>;
        return str.find_first_of(prefixes) == 0;
    }

    template<typename charT, typename charTrait, typename U, std::enable_if_t<std::is_constructible_v<std::basic_string<charT, charTrait>, const U*>, int> = 0>
    inline constexpr bool starts_of(
        const std::basic_string<charT, charTrait>& str,
        const U* prefixes
    )
    {
        return starts_of<charT, charTrait>(str, std::basic_string<charT, charTrait>(prefixes));
    }

    template<typename charT, typename charTrait>
    inline constexpr bool ends_of(
        const std::basic_string<charT, charTrait>& str, 
        const std::basic_string<charT, charTrait>& suffixes
    )
    {
        //using T = typename std::basic_string<charT, charTrait>;
        return str.find_last_of(suffixes) == (str.size() - 1);
    }

    template<typename charT, typename charTrait, typename U, std::enable_if_t<std::is_constructible_v<std::basic_string<charT, charTrait>, const U*>, int> = 0>
    inline constexpr bool ends_of(
        const std::basic_string<charT, charTrait>& str,
        const U* suffixes
    )
    {
        return ends_of<charT, charTrait>(str, std::basic_string<charT, charTrait>(suffixes));
    }

    template<typename charT, typename charTrait>
    inline constexpr int istrcmp(
        const std::basic_string<charT, charTrait>& str1,
        typename std::basic_string<charT, charTrait>::size_type str1_start,
        typename std::basic_string<charT, charTrait>::size_type char_count,
        const std::basic_string<charT, charTrait>& str2,
        typename std::basic_string<charT, charTrait>::size_type str2_start = 0
    )
    {
        using T = typename std::basic_string<charT, charTrait>;

        auto str1_mincount = char_count == T::npos ? 0 : char_count;
        auto str2_mincount = char_count == T::npos ? 0 : char_count;
        assert(str1.size() >= str1_start + str1_mincount);
        assert(str2.size() >= str2_start + str2_mincount);
        
        auto it1 = str1.begin() + str1_start;
        auto end1 = char_count == T::npos ? str1.end() : (str1.begin() + str1_start + str1_mincount);
        auto it2 = str2.begin() + str2_start;
        auto end2 = char_count == T::npos ? str2.end() : (str2.begin() + str2_start + str2_mincount);

        while (it1 != end1 && it2 != end2)
        {
            auto ch1 = static_cast<typename T::value_type>(std::tolower(*it1));
            auto ch2 = static_cast<typename T::value_type>(std::tolower(*it2));

            if (ch1 < ch2) return -1;
            if (ch1 > ch2) return 1;

            ++it1;
            ++it2;
        }

        if (it1 == end1)
            return it2 == end2 ? 0 : -1;
        else
            return 1;
    }

    template<typename charT, typename charTrait>
    inline constexpr int istrcmp(
        const std::basic_string<charT, charTrait>& str1,
        const std::basic_string<charT, charTrait>& str2
    )
    {
        using T = typename std::basic_string<charT, charTrait>;
        return istrcmp<charT, charTrait>(str1, 0, T::npos, str2, 0);
    }

    template<typename charT, typename charTrait, typename U, std::enable_if_t<std::is_constructible_v<std::basic_string<charT, charTrait>, const U*>, int> = 0>
    inline constexpr int istrcmp(
        const std::basic_string<charT, charTrait>& str1,
        typename std::basic_string<charT, charTrait>::size_type str1_start,
        typename std::basic_string<charT, charTrait>::size_type char_count,
        const U* str2,
        typename std::basic_string<charT, charTrait>::size_type str2_start = 0
    )
    {
        using T = typename std::basic_string<charT, charTrait>;
        
        if constexpr (std::is_same_v<std::decay_t<charT>, char>)
        {
            auto str2_len = std::strlen(str2);
            auto str2_minlen = char_count == T::npos ? 0 : char_count;
            assert(str2_len >= str2_start + str2_minlen);
        }
        else if constexpr (std::is_same_v<std::decay_t<charT>, wchar_t>)
        {
            auto str2_len = std::wcslen(str2);
            auto str2_minlen = char_count == T::npos ? 0 : char_count;
            assert(str2_len >= str2_start + str2_minlen);
        }

        return istrcmp<charT, charTrait>(str1, str1_start, char_count, T(str2 + str2_start));
    }

    template<typename charT, typename charTrait, typename U, std::enable_if_t<std::is_constructible_v<std::basic_string<charT, charTrait>, const U*>, int> = 0>
    inline constexpr int istrcmp(
        const std::basic_string<charT, charTrait>& str1,
        const U* str2
    )
    {
        using T = typename std::basic_string<charT, charTrait>;
        return istrcmp<charT>(str1, T(str2));
    }

    template<typename charT, typename charTrait>
    inline std::basic_string<charT, charTrait>& replace_inplace(
        std::basic_string<charT, charTrait>& str,
        const std::basic_string<charT, charTrait>& from,
        const std::basic_string<charT, charTrait>& to,
        typename std::basic_string<charT, charTrait>::size_type start = 0
    )
    {
        using T = typename std::basic_string<charT, charTrait>;
        assert(from.size() > 0);
        assert(start < str.size());

        if (str.empty() || from == to)
            return str;
        
        if (from.size() == to.size())
        {
            typename T::size_type len = from.size();
            typename T::size_type pos = str.find(from, start);

            while (pos != T::npos) 
            {
                str.replace(pos, len, to);
                pos = str.find(from, pos + len);
            }
        }
        else
        {
            T result;
            result.append(str, 0, start);
            typename T::size_type pos = 0;

            do 
            {
                pos = str.find(from, start);
                if (pos != T::npos) 
                {
                    result.append(str, start, pos - start);
                    result.append(to);
                    start = pos + from.size();
                }
                else 
                {
                    result.append(str, start, str.size() - start);
                }
                
            } while (pos != T::npos);

            str.swap(result);
        }
        
        return str;
    }

    template<typename charT, typename charTrait, typename U, std::enable_if_t<std::is_constructible_v<std::basic_string<charT, charTrait>, const U*>, int> = 0>
    inline std::basic_string<charT, charTrait>& replace_inplace(
        std::basic_string<charT, charTrait>& str,
        const U* from,
        const U* to,
        typename std::basic_string<charT, charTrait>::size_type start = 0
    )
    {
        return replace_inplace<charT, charTrait>(str, std::basic_string<charT, charTrait>(from), std::basic_string<charT, charTrait>(to), start);
    }

    template<typename charT, typename charTrait>
    inline std::basic_string<charT, charTrait>& replace_inplace(
        std::basic_string<charT, charTrait>& str,
        const charT& from,
        const charT& to,
        typename std::basic_string<charT, charTrait>::size_type start = 0
    )
    {
        if (from != to)
        {
            for (int i = (int)start; i < (int)str.size(); i++)
            {
                if (str[i] == from)
                    str[i] = to;
            }
        }

        return str;
    }

    template<typename charT, typename charTrait>
    inline std::vector<std::basic_string<charT, charTrait>> split(
        const std::basic_string<charT, charTrait>& str,
        const std::basic_string<charT, charTrait>& delims,
        bool ignoreEmpty = false
    )
    {
        using T = typename std::basic_string<charT, charTrait>;
        typename T::size_type first = 0;
        
        T str_dup(str);

        for (const charT& ch : delims)
        {
            replace_inplace(str_dup, ch, delims[0]);
        }
        
        std::basic_istringstream<charT, charTrait> sstream(str_dup);
        std::vector<T> result;
        T token;
        
        while (std::getline<charT, charTrait>(sstream, token, delims[0]))
        {
            if (token.empty() && ignoreEmpty)
                continue;

            result.push_back(token);
        }

        if (str_dup.back() == delims[0] && !ignoreEmpty)
        {
            result.push_back(T());
        }

        return result;
    }

    template<typename charT, typename charTrait, typename U, std::enable_if_t<std::is_constructible_v<std::basic_string<charT, charTrait>, const U*>, int> = 0>
    inline std::vector<std::basic_string<charT, charTrait>> split(
        const std::basic_string<charT, charTrait>& str,
        const U* delims,
        bool ignoreEmpty = false
    )
    {
        return split<charT, charTrait>(str, std::basic_string<charT, charTrait>(delims), ignoreEmpty);
    }
}