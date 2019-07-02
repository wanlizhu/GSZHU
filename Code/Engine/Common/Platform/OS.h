#pragma once
#include "Common/Config.h"
#include "Common/BasicTypes.h"
#include <string>

namespace ZHU
{
    template<>
    std::string Convert<std::string, std::wstring_view>(const std::wstring_view& wstr);
    template<>
    std::wstring Convert<std::wstring, std::string_view>(const std::string_view& str);

    std::string GetCurrentFileName(std::string* dir = nullptr, std::string* basename = nullptr, std::string* ext = nullptr);
}