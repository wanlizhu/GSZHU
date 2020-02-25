#pragma once

#include <string>
#include <vector>
#include <codecvt>
#include <locale>
#include <algorithm>
#include <sstream>
#include "Core/Common.h"

namespace Wanli
{
    void WANLI_API ToString(const std::wstring_view& wstr, std::string& result);
    void WANLI_API ToWstring(const std::string_view& str, std::wstring& result);
    bool WANLI_API StartsWith(const std::string_view& str, const std::string_view& prefix);
    bool WANLI_API EndsWith(const std::string_view& str, const std::string_view& suffix);
    void WANLI_API Split(const std::string_view& str, char sep, std::vector<std::string>& result);
    void WANLI_API Trim(std::string& str, const std::string_view& whites);
    void WANLI_API ReplaceAll(std::string& str, const std::string_view& token, const std::string_view& to);
    void WANLI_API Lowercase(std::string& str);
    void WANLI_API Uppercase(std::string& str);

    template<typename... Args>
    inline constexpr std::string 
        FormatString(const char* format, Args... args)
    {
        constexpr size_t bufferLen = 1024;
        static char buffer[bufferLen];

        size_t newLen = snprintf(&buffer[0], bufferLen, format, args...);
        newLen++; // include the '\0'

        // stack buffer, create a heap buffer
        if (newLen > bufferLen)
        {
            std::vector<char> buffer2(newLen);
            snprintf(buffer2.data(), newLen, format, args...);
            return std::string(buffer2.data());
        }

        return std::string(buffer);
    }
}