#include "Wanlix/Utility/Strings.h"
#include <codecvt>

namespace Wanlix
{
    std::wstring ToWString(const std::string& source)
    {
        std::wstring_convert<std::codecvt_utf8<wchar_t>> conv;
        return conv.from_bytes(source);
    }

    template<> 
    std::string ToString<std::wstring>(const std::wstring& source)
    {
        std::wstring_convert<std::codecvt_utf8<wchar_t>> conv;
        return conv.to_bytes(source);
    }

    void StringUtil::Trim(std::string& str, bool left, bool right)
    {
        static const std::string delims = " \t\r\n";
        return StringUtil::Trim(str, delims, left, right);
    }

    void StringUtil::Trim(std::wstring& str, bool left, bool right)
    {
        static const std::wstring delims = L" \t\r\n";
        return StringUtil::Trim(str, delims, left, right);
    }

    void StringUtil::Trim(std::string& str, const std::string& delims, bool left, bool right)
    {
        if (right) {
            str.erase(str.find_last_not_of(delims) + 1);
        }
        if (left) {
            str.erase(0, str.find_first_not_of(delims));
        }
    }

    void StringUtil::Trim(std::wstring& str, const std::wstring& delims, bool left, bool right)
    {
        if (right) {
            str.erase(str.find_last_not_of(delims) + 1);
        }
        if (left) {
            str.erase(0, str.find_first_not_of(delims));
        }
    }

    template<typename _String_>
    static std::vector<_String_> SplitInternal(const _String_& str, 
                                               const _String_& delims,
                                               uint32_t maxSplits)
    {
        std::vector<_String_> ret;
        // Pre-allocate some space for performance
        ret.reserve(maxSplits ? maxSplits + 1 : 10);    // 10 is guessed capacity for most case

        unsigned int numSplits = 0;

        // Use STL methods
        size_t start, pos;
        start = 0;
        do 
        {
            pos = str.find_first_of(delims, start);
            if (pos == start) {
                start = pos + 1; // Do nothing
            }
            else if (pos == _String_::npos || (maxSplits && numSplits == maxSplits)) {
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

    StringUtil::StringList StringUtil::Split(const std::string& str, const std::string& delims, uint32_t maxSplits)
    {
        return SplitInternal(str, delims, maxSplits);
    }

    StringUtil::WStringList StringUtil::Split(const std::wstring& str, const std::wstring& delims, uint32_t maxSplits)
    {
        return SplitInternal(str, delims, maxSplits);
    }

    template<typename _String_>
    static std::vector<_String_> TokenizeInternal(const _String_& str, 
                                                  const _String_& singleDelims,
                                                  const _String_& doubleDelims,
                                                  unsigned int maxSplits)
    {
        std::vector<_String_> ret;
        // Pre-allocate some space for performance
        ret.reserve(maxSplits ? maxSplits + 1 : 10);    // 10 is guessed capacity for most case

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
            else if (pos == _String_::npos || (maxSplits && numSplits == maxSplits)) {
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

    StringUtil::StringList StringUtil::Tokenize(const std::string& str,
                                                const std::string& delims ,
                                                const std::string& doubleDelims,
                                                unsigned int maxSplits)
    {
        return TokenizeInternal(str, delims, doubleDelims, maxSplits);
    }

    StringUtil::WStringList StringUtil::Tokenize(const std::wstring& str,
                                                 const std::wstring& delims,
                                                 const std::wstring& doubleDelims,
                                                 unsigned int maxSplits)
    {
        return TokenizeInternal(str, delims, doubleDelims, maxSplits);
    }

    void StringUtil::ToLower(std::string& str)
    {
        std::transform(str.begin(), str.end(), str.begin(), tolower);
    }

    void StringUtil::ToLower(std::wstring& str)
    {
        std::transform(str.begin(), str.end(), str.begin(), tolower);
    }

    void StringUtil::ToUpper(std::string& str)
    {
        std::transform(str.begin(), str.end(), str.begin(), toupper);
    }

    void StringUtil::ToUpper(std::wstring& str)
    {
        std::transform(str.begin(), str.end(), str.begin(), toupper);
    }

    template<typename _String_>
    static bool StartsWithInternal(const _String_& str, const _String_& pattern, bool caseSensitive)
    {
        size_t thisLen = str.length();
        size_t patternLen = pattern.length();
        if (thisLen < patternLen || patternLen == 0)
            return false;

        _String_ startOfThis = str.substr(0, patternLen);
        if (!caseSensitive) {
            _String_ patternDup = pattern;
            StringUtil::ToLower(patternDup);
            StringUtil::ToLower(startOfThis);
            return startOfThis == patternDup;
        }

        return startOfThis == pattern;
    }

    bool StringUtil::StartsWith(const std::string& str, const std::string& pattern, bool caseSensitive)
    {
        return StartsWithInternal(str, pattern, caseSensitive);
    }

    bool StringUtil::StartsWith(const std::wstring& str, const std::wstring& pattern, bool caseSensitive)
    {
        return StartsWithInternal(str, pattern, caseSensitive);
    }

    template<typename _String_>
    static bool EndsWithInternal(const _String_& str, const _String_& pattern, bool caseSensitive)
    {
        size_t thisLen = str.length();
        size_t patternLen = pattern.length();
        if (thisLen < patternLen || patternLen == 0)
            return false;

        _String_ endOfThis = str.substr(thisLen - patternLen, patternLen);
        if (!caseSensitive) {
            _String_ patternDup = pattern;
            StringUtil::ToLower(patternDup);
            StringUtil::ToLower(endOfThis);
            return endOfThis == patternDup;
        }

        return endOfThis == pattern;
    }

    bool StringUtil::EndsWith(const std::string& str, const std::string& pattern, bool caseSensitive)
    {
        return EndsWithInternal(str, pattern, caseSensitive);
    }

    bool StringUtil::EndsWith(const std::wstring& str, const std::wstring& pattern, bool caseSensitive)
    {
        return EndsWithInternal(str, pattern, caseSensitive);
    }

    template<typename _String_>
    static bool MatchInternal(const _String_& str, const _String_& pattern, bool caseSensitive)
    {
        _String_ tmpStr = str;
        _String_ tmpPattern = pattern;
        if (!caseSensitive)
        {
            StringUtil::ToLower(tmpStr);
            StringUtil::ToLower(tmpPattern);
        }

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

    bool StringUtil::Match(const std::string& str, const std::string& pattern, bool caseSensitive)
    {
        return MatchInternal(str, pattern, caseSensitive);
    }

    bool StringUtil::Match(const std::wstring& str, const std::wstring& pattern, bool caseSensitive)
    {
        return MatchInternal(str, pattern, caseSensitive);
    }

    template<typename _String_>
    static _String_ ReplaceAllInternal(const _String_& source,
                                       const _String_& replaceWhat, 
                                       const _String_& replaceWithWhat)
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

    std::string StringUtil::ReplaceAll(const std::string& str, 
                                       const std::string& replaceWhat,
                                       const std::string& replaceWithWhat)
    {
        return ReplaceAllInternal(str, replaceWhat, replaceWithWhat);
    }

    std::wstring StringUtil::ReplaceAll(const std::wstring& str, 
                                        const std::wstring& replaceWhat,
                                        const std::wstring& replaceWithWhat)
    {
        return ReplaceAllInternal(str, replaceWhat, replaceWithWhat);
    }
}