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
}