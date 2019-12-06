#pragma once

#include <string>
#include <algorithm>
#include <vector>
#include <cstdint>

namespace Wanlix
{
    std::string ToString(const wchar_t* wstr, int size = -1);
    std::wstring ToWstring(const char* str, int size = -1);

    class String
    {
    public:
        static void Trim(std::string& str, bool left = true, bool right = true);
        static void Trim(std::wstring& str, bool left = true, bool right = true);
        static void Trim(std::string& str, const char* delims, bool left = true, bool right = true);
        static void Trim(std::wstring& str, const wchar_t* delims, bool left = true, bool right = true);
    
        static void ToLower(std::string& str);
        static void ToLower(std::wstring& str);

        static void ToUpper(std::string& str);
        static void ToUpper(std::wstring& str);

        static bool StartsWith(const std::string& str, const char* pattern);
        static bool StartsWith(const std::wstring& str, const wchar_t* pattern);

        static bool EndsWith(const std::string& str, const char* pattern);
        static bool EndsWith(const std::wstring& str, const wchar_t* pattern);

        static bool Match(const std::string& str, const char* pattern);
        static bool Match(const std::wstring& str, const wchar_t* pattern);

        static void Replace(std::string& str, const char* oldstr, const char* newstr);
        static void Replace(std::wstring& str, const wchar_t* oldstr, const wchar_t* newstr);

        static std::string  Join(const char* delim, const std::vector<std::string>& strs);
        static std::wstring Join(const wchar_t* delim, const std::vector<std::wstring>& strs);

        static std::vector<std::string>  Split(const std::string& str, const char* delims = "\t\n ");
        static std::vector<std::wstring> Split(const std::wstring& str, const wchar_t* delims = L"\t\n ");

        static std::vector<std::string>  Tokenize(const std::string& str, const char* delims = "\t\n ", const char* doubleDelims = "\"");
        static std::vector<std::wstring> Tokenize(const std::wstring& str, const wchar_t* delims = L"\t\n ", const wchar_t* doubleDelims = L"\"");
    };
}