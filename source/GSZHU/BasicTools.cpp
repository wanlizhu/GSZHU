#include <GSZHU/BasicTools.h>
#include <string>
#include <sstream>
#include <locale>
#include <algorithm>
#include <cctype>
#include <assert.h>

namespace GSZHU {
    std::string ToString(const std::wstring& WStr) {
        std::string Str;

        const std::ctype<wchar_t>& ctfacet = std::use_facet<std::ctype<wchar_t>>(std::wstringstream().getloc());
        for (auto WChar = WStr.begin(); WChar != WStr.end(); WChar++) {
            Str.push_back(ctfacet.narrow(*WChar, 0));
        }

        return Str;
    }

    std::wstring ToWString(const std::string& Str) {
        std::wstring WStr;

        const std::ctype<wchar_t>& ctfacet = std::use_facet<std::ctype<wchar_t>>(std::wstringstream().getloc());
        for (auto WChar = Str.begin(); WChar != Str.end(); WChar++) {
            WStr.push_back(ctfacet.widen(*WChar));
        }

        return WStr;
    }
}