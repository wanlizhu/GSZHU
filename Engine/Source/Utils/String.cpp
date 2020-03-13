#include "Utils/String.h"

namespace Wanli
{
    void ToString(const std::wstring_view& wstr, std::string& result)
    {
        result = std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>, wchar_t>().to_bytes(wstr.data());
    }

    void ToWstring(const std::string_view& str, std::wstring& result)
    {
        result = std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>, wchar_t>().from_bytes(str.data());
    }

    bool StartsWith(const std::string_view& str, const std::string_view& prefix)
    {
        if (str.size() >= prefix.size())
        {
            return str.compare(0, prefix.size(), prefix) == 0;
        }
        return false;
    }

    bool EndsWith(const std::string_view& str, const std::string_view& suffix)
    {
        if (str.size() >= suffix.size())
        {
            return str.compare(str.size() - suffix.size(), suffix.size(), suffix) == 0;
        }
        return false;
    }

    void Split(const std::string_view& str, char sep, std::vector<std::string>& result)
    {
        std::string token;
        std::istringstream stream(str.data());
        while (std::getline(stream, token, sep))
        {
            result.push_back(token);
        }
    }

    void Trim(std::string& str, const std::string_view& whites)
    {
        auto beg = str.find_first_not_of(whites);
        if (beg == std::string::npos)
        {
            str = "";
            return;
        }

        auto end = str.find_last_not_of(whites);
        auto range = end - beg + 1;
        str = str.substr(beg, range);
    }

    void ReplaceAll(std::string& str, const std::string_view& token, const std::string_view& to)
    {
        auto pos = str.find(token);
        while (pos != std::string::npos)
        {
            str.replace(pos, token.size(), to);
            pos = str.find(token, pos + token.size());
        }
    }

    void Lowercase(std::string& str)
    {
        std::transform(str.begin(), str.end(), str.begin(), tolower);
    }

    void Uppercase(std::string& str)
    {
        std::transform(str.begin(), str.end(), str.begin(), toupper);
    }
}