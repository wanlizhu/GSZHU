#include "dejavu/core/strings.h"
#if defined(_WIN32)
#include <Windows.h>
#else
#endif

namespace djv
{
#if defined(_WIN32)

    std::string DJV_API to_string(const std::wstring& wstr)
    {
        if (wstr.empty()) {
            return std::string();
        }

        int size_needed = ::WideCharToMultiByte(CP_UTF8, 0, &wstr[0], (int)wstr.size(), NULL, 0, NULL, NULL);
        std::string strTo(size_needed, 0);
        ::WideCharToMultiByte(CP_UTF8, 0, &wstr[0], (int)wstr.size(), &strTo[0], size_needed, NULL, NULL);
        return strTo;
    }

    std::wstring DJV_API to_wstring(const std::string& str)
    {
         if (str.empty()) {
             return std::wstring();
         }
        
        int size_needed = ::MultiByteToWideChar(CP_UTF8, 0, &str[0], (int)str.size(), NULL, 0);
        std::wstring wstrTo(size_needed, 0);
        ::MultiByteToWideChar(CP_UTF8, 0, &str[0], (int)str.size(), &wstrTo[0], size_needed);
        return wstrTo;
    }

#else
#endif
}