#pragma once

#include "Wanlix/Core3D/Types.h"

namespace Wanlix
{
    std::wstring ToWString(StringCRef source);

    template<typename T>
    String ToString(const T& val) {
        static_assert(false);
    }

    template<> 
    String ToString<WString>(WStringCRef source);

    class StringUtil
    {
    public:
        // Removes any whitespace characters from beginning or end of the string
        static void Trim(
            String& str,
            bool    left = true,
            bool    right = true
        );
        static void Trim(
            WString& str,
            bool     left = true,
            bool     right = true
        );

        // Removes specified characters from beginning or end of the string
        static void Trim(
            String&    str,
            StringCRef delims,
            bool       left = true,
            bool       right = true
        );
        static void Trim(
            WString&    str,
            WStringCRef delims,
            bool        left = true,
            bool        right = true
        );
    
        /**
         * Returns a vector of strings containing all the substrings delimited by the provided delimiter characters.
         *
         * @param[in]	str		 	The string to split.
         * @param[in]	delims   	(optional) Delimiter characters to split the string by. They will not
         * 							be included in resulting substrings.
         * @param[in]	maxSplits	(optional) The maximum number of splits to perform (0 for unlimited splits). If this
         *							parameters is > 0, the splitting process will stop after this many splits, left to right.
         */
        static StringList Split(
            StringCRef str,
            StringCRef delims = "\t\n ",
            uint32_t   maxSplits = 0
        );
        static WStringList Split(
            WStringCRef str,
            WStringCRef delims = L"\t\n ",
            uint32_t    maxSplits = 0
        );

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
        static StringList Tokenize(
            StringCRef str, 
            StringCRef delims = "\t\n ",
            StringCRef doubleDelims = "\"", 
            uint32_t   maxSplits = 0
        );
        static WStringList Tokenize(
            WStringCRef str, 
            WStringCRef delims = L"\t\n ",
            WStringCRef doubleDelims = L"\"", 
            uint32_t    maxSplits = 0
        );

        static void ToLower(String& str);
        static void ToLower(WString& str);

        static void ToUpper(String& str);
        static void ToUpper(WString& str);

        /**
         * Returns whether the string begins with the pattern passed in.
         *
         * @param[in]	str		 	    String to compare.
         * @param[in]	pattern		    Pattern to compare with.
         * @param[in]	caseSensitive	(optional) Should the match be case sensitive or not.
         */
        static bool StartsWith(
            StringCRef str,
            StringCRef pattern,
            bool       caseSensitive = true
        );
        static bool StartsWith(
            WStringCRef str,
            WStringCRef pattern,
            bool        caseSensitive = true
        );

        /**
         * Returns whether the string end with the pattern passed in.
         *
         * @param[in]	str		 	    String to compare.
         * @param[in]	pattern		    Pattern to compare with.
         * @param[in]	caseSensitive	(optional) Should the match be case sensitive or not.
         */
        static bool EndsWith(
            StringCRef str,
            StringCRef pattern,
            bool       caseSensitive = true
        );
        static bool EndsWith(
            WStringCRef str,
            WStringCRef pattern,
            bool        caseSensitive = true
        );

        /**
         * Returns true if the string matches the provided pattern. Pattern may use a "*" wildcard for matching any
         * characters.
         *
         * @param[in]	str			 	The string to test.
         * @param[in]	pattern		 	Patterns to look for.
         * @param[in]	caseSensitive	(optional) Should the match be case sensitive or not.
         */
        static bool Match(
            StringCRef str,
            StringCRef pattern,
            bool       caseSensitive = true
        );
        static bool Match(
            WStringCRef str,
            WStringCRef pattern, 
            bool        caseSensitive = true
        );

        /**
         * Replace all instances of a substring with a another substring.
         *
         * @param[in]	source  String to search.
         * @param[in]	strOld  Substring to find and replace
         * @param[in]	strNew	Substring to replace with (the new sub-string)
         *
         * @return	An updated string with the substrings replaced.
         */
        static String ReplaceAll(
            StringCRef str,
            StringCRef replaceWhat,
            StringCRef replaceWithWhat
        );
        static std::wstring ReplaceAll(
            WStringCRef str,
            WStringCRef replaceWhat,
            WStringCRef replaceWithWhat
        );
    };
}