#include "Wanlix/Strings.h"
#include <codecvt>
#if defined _WIN32
#include <Windows.h>
#endif

namespace Wanlix
{
#if defined _WIN32
    std::string ToString(const wchar_t* wstr, int size)
    {
        if (wstr == nullptr) {
            return std::string();
        }

        size = size < 0 ? wcslen(wstr) : size;
        int sizeNeeded = WideCharToMultiByte(CP_UTF8, 0, wstr, size, NULL, 0, NULL, NULL);
        
        std::string result(sizeNeeded, '\0');
        WideCharToMultiByte(CP_UTF8, 0, wstr, size, &result[0], sizeNeeded, NULL, NULL);
        
        return result;
    }

    std::wstring ToWstring(const char* str, int size)
    {
        if (str == nullptr) {
            return std::wstring();
        }

        size = size < 0 ? strlen(str) : size;
        int sizeNeeded = MultiByteToWideChar(CP_UTF8, 0, str, size, NULL, 0);
        
        std::wstring result(sizeNeeded, '\0');
        MultiByteToWideChar(CP_UTF8, 0, str, size, &result[0], sizeNeeded);
        
        return result;
    }
#else
    std::wstring ToWString(const char* str, int size)
    {
        std::wstring_convert<std::codecvt_utf8<wchar_t>> conv;
        size = size < 0 ? strlen(str) : size;
        return conv.from_bytes(str, str + size);
    }

    std::string ToString(const wchar_t* wstr, int size)
    {
        std::wstring_convert<std::codecvt_utf8<wchar_t>> conv;
        size = size < 0 ? wcslen(wstr) : size;
        return conv.to_bytes(wstr, wstr + size);
    }
#endif

    void String::Trim(std::string& str, bool left, bool right)
    {
        static const char* delims = " \t\r\n";
        return String::Trim(str, delims, left, right);
    }

    void String::Trim(std::wstring& str, bool left, bool right)
    {
        static const wchar_t* delims = L" \t\r\n";
        return String::Trim(str, delims, left, right);
    }

    void String::Trim(std::string& str, const char* delims, bool left, bool right)
    {
        if (right) {
            str.erase(str.find_last_not_of(delims) + 1);
        }
        if (left) {
            str.erase(0, str.find_first_not_of(delims));
        }
    }

    void String::Trim(std::wstring& str, const wchar_t* delims, bool left, bool right)
    {
        if (right) {
            str.erase(str.find_last_not_of(delims) + 1);
        }
        if (left) {
            str.erase(0, str.find_first_not_of(delims));
        }
    }

    template<typename _String_>
    static std::vector<_String_> SplitInternal(const _String_& str, const _String_& delims)
    {
        std::vector<_String_> ret;
        unsigned int numSplits = 0;

        size_t start, pos;
        start = 0;
        do 
        {
            pos = str.find_first_of(delims, start);
            if (pos == start) {
                start = pos + 1; // Do nothing
            }
            else if (pos == _String_::npos) {
                ret.push_back(str.substr(start)); // Copy the rest of the string
                break;
            }
            else {
                ret.push_back(str.substr(start, pos - start)); // Copy up to delimiter
                start = pos + 1;
            }
            // parse up to next real data
            start = str.find_first_not_of(delims, start);
            ++numSplits;

        } while (pos != _String_::npos);

        return ret;
    }

    std::vector<std::string> String::Split(const std::string& str, const char* delims)
    {
        return SplitInternal<std::string>(str, delims);
    }

    std::vector<std::wstring> String::Split(const std::wstring& str, const wchar_t* delims)
    {
        return SplitInternal<std::wstring>(str, delims);
    }

    template<typename _String_>
    static std::vector<_String_> TokenizeInternal(const _String_& str,  const _String_& singleDelims, const _String_& doubleDelims)
    {
        std::vector<_String_> ret;

        unsigned int numSplits = 0;
        _String_ delims = singleDelims + doubleDelims;

        // Use STL methods
        size_t start, pos;
        typename _String_::value_type curDoubleDelim = 0;
        start = 0;
        do
        {
            if (curDoubleDelim != 0) {
                pos = str.find(curDoubleDelim, start);
            }
            else {
                pos = str.find_first_of(delims, start);
            }

            if (pos == start) {
                typename _String_::value_type curDelim = str.at(pos);
                if (doubleDelims.find_first_of(curDelim) != _String_::npos) {
                    curDoubleDelim = curDelim;
                }
                // Do nothing
                start = pos + 1;
            }
            else if (pos == _String_::npos) {
                if (curDoubleDelim != 0) {
                    //Missing closer. Warn or throw exception?
                }
                // Copy the rest of the string
                ret.push_back(str.substr(start));
                break;
            }
            else {
                if (curDoubleDelim != 0) {
                    curDoubleDelim = 0;
                }

                // Copy up to delimiter
                ret.push_back(str.substr(start, pos - start));
                start = pos + 1;
            }
            if (curDoubleDelim == 0) {
                // parse up to next real data
                start = str.find_first_not_of(singleDelims, start);
            }

            ++numSplits;

        } while (pos != _String_::npos);

        return ret;
    }

    std::vector<std::string> String::Tokenize(const std::string& str, const char* delims, const char* doubleDelims)
    {
        return TokenizeInternal<std::string>(str, delims, doubleDelims);
    }

    std::vector<std::wstring> String::Tokenize(const std::wstring& str, const wchar_t* delims, const wchar_t* doubleDelims)
    {
        return TokenizeInternal<std::wstring>(str, delims, doubleDelims);
    }

    void String::ToLower(std::string& str)
    {
        std::transform(str.begin(), str.end(), str.begin(), tolower);
    }

    void String::ToLower(std::wstring& str)
    {
        std::transform(str.begin(), str.end(), str.begin(), tolower);
    }

    void String::ToUpper(std::string& str)
    {
        std::transform(str.begin(), str.end(), str.begin(), toupper);
    }

    void String::ToUpper(std::wstring& str)
    {
        std::transform(str.begin(), str.end(), str.begin(), toupper);
    }

    template<typename _String_>
    static bool StartsWithInternal(const _String_& str, const _String_& pattern)
    {
        return str.find(pattern) == 0;
    }

    bool String::StartsWith(const std::string& str, const char* pattern)
    {
        return StartsWithInternal<std::string>(str, pattern);
    }

    bool String::StartsWith(const std::wstring& str, const wchar_t* pattern)
    {
        return StartsWithInternal<std::wstring>(str, pattern);
    }

    template<typename _String_>
    static bool EndsWithInternal(const _String_& str, const _String_& pattern)
    {
        if (str.size() < pattern.size()) {
            return false;
        }

        return _String_(str.c_str() + str.size() - size) == pattern;
    }

    bool String::EndsWith(const std::string& str, const char* pattern)
    {
        return EndsWithInternal<std::string>(str, pattern);
    }

    bool String::EndsWith(const std::wstring& str, const wchar_t* pattern)
    {
        return EndsWithInternal<std::wstring>(str, pattern);
    }

    template<typename _String_>
    static bool MatchInternal(const _String_& str, const _String_& pattern)
    {
        _String_ tmpStr = str;
        _String_ tmpPattern = pattern;

        typename _String_::const_iterator strIt = tmpStr.begin();
        typename _String_::const_iterator patIt = tmpPattern.begin();
        typename _String_::const_iterator lastWildCardIt = tmpPattern.end();
        while (strIt != tmpStr.end() && patIt != tmpPattern.end())
        {
            if (*patIt == '*') {
                lastWildCardIt = patIt;
                // Skip over looking for next character
                ++patIt;
                if (patIt == tmpPattern.end()) {
                    // Skip right to the end since * matches the entire rest of the string
                    strIt = tmpStr.end();
                }
                else {
                    // scan until we find next pattern character
                    while (strIt != tmpStr.end() && *strIt != *patIt)
                        ++strIt;
                }
            }
            else {
                if (*patIt != *strIt) {
                    if (lastWildCardIt != tmpPattern.end()) {
                        // The last wildcard can match this incorrect sequence
                        // rewind pattern to wildcard and keep searching
                        patIt = lastWildCardIt;
                        lastWildCardIt = tmpPattern.end();
                    }
                    else {
                        // no wildwards left
                        return false;
                    }
                }
                else {
                    ++patIt;
                    ++strIt;
                }
            }
        }

        // If we reached the end of both the pattern and the string, we succeeded
        return (patIt == tmpPattern.end() && strIt == tmpStr.end());
    }

    bool String::Match(const std::string& str, const char* pattern)
    {
        return MatchInternal<std::string>(str, pattern);
    }

    bool String::Match(const std::wstring& str, const wchar_t* pattern)
    {
        return MatchInternal<std::wstring>(str, pattern);
    }

    template<typename _String_>
    static _String_ ReplaceInternal(const _String_& source, const _String_& replaceWhat, const _String_& replaceWithWhat)
    {
        _String_ result = source;
        typename _String_::size_type pos = 0;
        while (1)
        {
            pos = result.find(replaceWhat, pos);
            if (pos == _String_::npos) break;
            result.replace(pos, replaceWhat.size(), replaceWithWhat);
            pos += replaceWithWhat.size();
        }
        return result;
    }

    void String::Replace(std::string& str, const char* replaceWhat, const char* replaceWithWhat)
    {
        str = ReplaceInternal<std::string>(str, replaceWhat, replaceWithWhat);
    }

    void String::Replace(std::wstring& str, const wchar_t* replaceWhat, const wchar_t* replaceWithWhat)
    {
        str = ReplaceInternal<std::wstring>(str, replaceWhat, replaceWithWhat);
    }

    template<typename _String_>
    static _String_ JointInternal(const _String_& delim, const std::vector<_String_>& strs)
    {
        if (strs.empty()) {
            return _String_("");
        }

        _String_ result;
        for (size_t i = 0; i < strs.size(); i++) {
            result += strs[i];
            if (i < (str.size() - 1)) {
                result += delim;
            }
        }
        return result;
    }

    std::string String::Join(const char* delim, const std::vector<std::string>& strs)
    {
        return JointInternal<std::string>(delim, strs);
    }

    std::wstring String::Join(const wchar_t* delim, const std::vector<std::wstring>& strs)
    {
        return JointInternal<std::wstring>(delim, strs);
    }
}