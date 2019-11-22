#pragma once

#include <string>
#include <vector>
#include <algorithm>
#include <unordered_map>

#define NASIZE ((size_t)-1)

namespace UT
{
    std::wstring ToWString(const std::string& source);

    template<typename T>
    std::string ToString(const T& val) {
        static_assert(false);
    }

    template<> 
    std::string ToString<std::wstring>(const std::wstring& source);


    class StringUtil
    {
    public:
        using StringList = std::vector<std::string>;
        using WStringList = std::vector<std::wstring>;
        struct Range
        {
            size_t offset = 0;
            size_t size = 0;

            Range() = default;
            Range(size_t a, size_t b) : offset(a), size(b) {}
            template<typename _String_>
            Range(const _String_& str) : offset(0), size(str.size()) {}
        };
        using RangeList = std::vector<Range>;

        // Removes any whitespace characters from beginning or end of the string
        static void Trim(std::string& str, bool left = true, bool right = true);
        static void Trim(std::wstring& str, bool left = true, bool right = true);

        // Removes specified characters from beginning or end of the string
        static void Trim(std::string& str, const std::string& delims, bool left = true, bool right = true);
        static void Trim(std::wstring& str, const std::wstring& delims, bool left = true, bool right = true);
    
        /**
         * Returns a vector of strings containing all the substrings delimited by the provided delimiter characters.
         *
         * @param[in]	str		 	The string to split.
         * @param[in]	delims   	(optional) Delimiter characters to split the string by. They will not
         * 							be included in resulting substrings.
         * @param[in]	maxSplits	(optional) The maximum number of splits to perform (0 for unlimited splits). If this
         *							parameters is > 0, the splitting process will stop after this many splits, left to right.
         */
        static StringList Split(const std::string& str, const std::string& delims = "\t\n ", uint32_t maxSplits = 0);
        static WStringList Split(const std::wstring& str, const std::wstring& delims = L"\t\n ", uint32_t maxSplits = 0);

        /**
         * Returns a vector of strings containing all the substrings delimited by the provided delimiter characters, or the
         * double delimiters used for including normal delimiter characters in the tokenized string.
         *
         * @param[in]	str		 		The string to split.
         * @param[in]	delims   		(optional) Delimiter characters to split the string by. They will not
         * 								be included in resulting substrings.
         * @param[in]	doubleDelims	(optional) Delimiter character you may use to surround other normal delimiters,
         *								in order to include them in the tokensized string.
         * @param[in]	maxSplits		(optional) The maximum number of splits to perform (0 for unlimited splits).
         *								If this parameters is > 0, the splitting process will stop after this many splits,
         *								left to right.
         */
        static StringList Tokenize(const std::string& str, 
                                   const std::string& delims = "\t\n ",
                                   const std::string& doubleDelims = "\"", 
                                   unsigned int maxSplits = 0);
        static WStringList Tokenize(const std::wstring& str, 
                                    const std::wstring& delims = L"\t\n ",
                                    const std::wstring& doubleDelims = L"\"", 
                                    unsigned int maxSplits = 0);

        static void ToLower(std::string& str);
        static void ToLower(std::wstring& str);

        static void ToUpper(std::string& str);
        static void ToUpper(std::wstring& str);

        /**
         * Returns whether the string begins with the pattern passed in.
         *
         * @param[in]	str		 	    String to compare.
         * @param[in]	pattern		    Pattern to compare with.
         * @param[in]	caseSensitive	(optional) Should the match be case sensitive or not.
         */
        static bool StartsWith(const std::string& str, const std::string& pattern, bool caseSensitive = true);
        static bool StartsWith(const std::wstring& str, const std::wstring& pattern, bool caseSensitive = true);

        /**
         * Returns whether the string end with the pattern passed in.
         *
         * @param[in]	str		 	    String to compare.
         * @param[in]	pattern		    Pattern to compare with.
         * @param[in]	caseSensitive	(optional) Should the match be case sensitive or not.
         */
        static bool EndsWith(const std::string& str, const std::string& pattern, bool caseSensitive = true);
        static bool EndsWith(const std::wstring& str, const std::wstring& pattern, bool caseSensitive = true);

        /**
         * Returns true if the string matches the provided pattern. Pattern may use a "*" wildcard for matching any
         * characters.
         *
         * @param[in]	str			 	The string to test.
         * @param[in]	pattern		 	Patterns to look for.
         * @param[in]	caseSensitive	(optional) Should the match be case sensitive or not.
         */
        static bool Match(const std::string& str, const std::string& pattern, bool caseSensitive = true);
        static bool Match(const std::wstring& str, const std::wstring& pattern, bool caseSensitive = true);

        /**
         * Replace all instances of a substring with a another substring.
         *
         * @param[in]	source  String to search.
         * @param[in]	strOld  Substring to find and replace
         * @param[in]	strNew	Substring to replace with (the new sub-string)
         *
         * @return	An updated string with the substrings replaced.
         */
        static std::string ReplaceAll(const std::string& str,
                                      const std::string& replaceWhat,
                                      const std::string& replaceWithWhat);
        static std::wstring ReplaceAll(const std::wstring& str,
                                       const std::wstring& replaceWhat,
                                       const std::wstring& replaceWithWhat);
    };
}