#include "Common/Utilities.h"

namespace NTGS {
    std::wstring CodeConvert(const std::string& str) {
        std::wstring_convert<std::codecvt<wchar_t, char, std::mbstate_t>> 
            decoder(new std::codecvt<wchar_t, char, std::mbstate_t>("CHS"));
        std::wstring wstr = decoder.from_bytes(str);
        return wstr;
    }
}