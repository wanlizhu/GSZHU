#pragma once

#include <string>
#include <vector>
#include <codecvt>
#include <locale>
#include <algorithm>
#include <sstream>
#include "Core/BasicTypes.h"

namespace Wanli
{
    void DLLDECL ToString(const std::wstring_view& wstr, std::string& result);
    void DLLDECL ToWstring(const std::string_view& str, std::wstring& result);
    bool DLLDECL StartsWith(const std::string_view& str, const std::string_view& prefix);
    bool DLLDECL EndsWith(const std::string_view& str, const std::string_view& suffix);
    void DLLDECL Split(const std::string_view& str, char sep, std::vector<std::string>& result);
    void DLLDECL Trim(std::string& str, const std::string_view& whites);
    void DLLDECL ReplaceAll(std::string& str, const std::string_view& token, const std::string_view& to);
    void DLLDECL Lowercase(std::string& str);
    void DLLDECL Uppercase(std::string& str);

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