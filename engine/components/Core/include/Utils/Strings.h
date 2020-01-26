#pragma once

#include <string>
#include <algorithm>
#include <vector>
#include <cstdint>
#include <codecvt>
#include "Wanlix/Config.h"
#include "Wanlix/Common.h"

namespace Wanlix
{
    inline std::string ToString(const wchar_t* wstr, int size = -1) 
    {
        std::wstring_convert<std::codecvt_utf8<wchar_t>> conv;
        size = size < 0 ? (int)wcslen(wstr) : size;
        return conv.to_bytes(wstr, wstr + size);
    }
    
    inline std::wstring ToWstring(const char* str, int size = -1)
    {
        std::wstring_convert<std::codecvt_utf8<wchar_t>> conv;
        size = size < 0 ? (int)strlen(str) : size;
        return conv.from_bytes(str, str + size);
    }

    inline std::string ToLower(const std::string& str)
    {
        std::string data;
        std::transform(str.begin(),
                       str.end(),
                       data.begin(),
                       [] (unsigned char c) { 
                           return std::tolower(c);
                       });
        return data;
    }

    inline std::string Utf32ToUtf8(uint32_t codepoint)
    {
        #ifdef _WIN32
        std::wstring_convert<std::codecvt_utf8<uint32_t>, uint32_t> cvt;
        #else
        std::wstring_convert<std::codecvt_utf8<char32_t>, char32_t> cvt;
        #endif
        return cvt.to_bytes(codepoint);
    }

    inline bool StartsWith(const std::string& str, const std::string& prefix, bool caseSensitive = true)
    {
        if (str.size() >= prefix.size())
        {
            if (caseSensitive == false)
            {
                std::string s = str;
                std::string pfx = prefix;
                std::transform(str.begin(), str.end(), s.begin(), ::tolower);
                std::transform(prefix.begin(), prefix.end(), pfx.begin(), ::tolower);
                return s.compare(0, pfx.length(), pfx) == 0;
            }
            else
            {
                return str.compare(0, prefix.length(), prefix) == 0;
            }
        }
        return false;
    }

    inline bool EndsWith(const std::string& str, const std::string& suffix, bool caseSensitive = true)
    {
        if (str.size() >= suffix.size())
        {
            std::string s = str.substr(str.length() - suffix.length());
            if (caseSensitive == false)
            {
                std::string sfx = suffix;
                std::transform(s.begin(), s.end(), s.begin(), ::tolower);
                std::transform(sfx.begin(), sfx.end(), sfx.begin(), ::tolower);
                return (sfx == s);
            }
            else
            {
                return (s == suffix);
            }
        }
        return false;
    }

    inline std::vector<std::string> SplitString(const std::string& str, const std::string& delims)
    {
        std::string s;
        std::vector<std::string> vec;
        for (char c : str)
        {
            if (delims.find(c) != std::string::npos)
            {
                if (s.length())
                {
                    vec.push_back(s);
                    s.clear();
                }
            }
            else
            {
                s += c;
            }
        }
        if (s.length())
        {
            vec.push_back(s);
        }
        return vec;
    }

    inline std::string JoinStrings(const std::vector<std::string>& strings, const std::string& separator)
    {
        std::string result;
        for (auto it = strings.begin(); it != strings.end(); it++)
        {
            result += *it;

            if (it != strings.end() - 1)
            {
                result += separator;
            }
        }
        return result;
    }

    inline std::string TrimString(const std::string& str, bool trimLeft = true, bool trimRight = true)
    {
        std::string ret = str;
        if (trimLeft) 
        {
            size_t offset = ret.find_first_not_of(" \n\r\t");
            if (offset != std::string::npos)
            {
                ret = ret.substr(offset);
            }
        }
        if (trimRight)
        {
            size_t offset = ret.find_last_not_of(" \n\r\t");
            if (offset != std::string::npos)
            {
                ret = ret.substr(0, offset + 1);
            }
        }
        return ret;
    }

    inline std::string ReplaceString(const std::string& input, const std::string& src, const std::string& dst)
    {
        std::string res = input;
        size_t offset = res.find(src);
        while (offset != std::string::npos)
        {
            res.replace(offset, src.length(), dst);
            offset += dst.length();
            offset = res.find(src, offset);
        }
        return res;
    }

    inline std::string ConcatCommandLine(int argc, char** argv)
    {
        std::string str;
        for (int i = 0; i < argc; i++)
        {
            str += std::string(argv[i]) + ((i < argc - 1) ? " " : "");
        }
        return str;
    }
}