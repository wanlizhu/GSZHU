#include "ZHUENGINE_PCH.h"
#include "./OS.h"
#include <filesystem>

namespace ZHU
{
    template<>
    std::string Convert<std::string, std::wstring_view>(const std::wstring_view& wstr)
    {
        std::string str;
        int required = ::WideCharToMultiByte(CP_UTF8, 0, wstr.data(), -1, nullptr, 0, 0, 0);
        if (required > 0) {
            str.resize(required);
            ::WideCharToMultiByte(CP_UTF8, 0, wstr.data(), -1, &str[0], required, 0, 0);
        }
        return str;
    }

    template<>
    std::wstring Convert<std::wstring, std::string_view>(const std::string_view& str)
    {
        std::wstring wstr;
        int required = ::MultiByteToWideChar(CP_UTF8, 0, str.data(), -1, nullptr, 0);
        if (required > 0) {
            wstr.resize(required);
            ::MultiByteToWideChar(CP_UTF8, 0, str.data(), -1, &wstr[0], required);
        }
        return wstr;
    }

    std::string GetCurrentFileName(std::string* dir, std::string* basename, std::string* ext)
    {
        char szPath[MAX_PATH];
        ::GetModuleFileNameA(NULL, szPath, MAX_PATH);
        
        auto path = std::filesystem::path(szPath).make_preferred();
        if (dir)
            *dir = path.parent_path().string();
        if (basename)
            *basename = path.stem().string();
        if (ext)
            *ext = path.extension().string();
        return path.string();
    }
}