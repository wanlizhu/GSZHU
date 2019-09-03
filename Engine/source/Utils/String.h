#pragma once

#include <string>
#include <vector>
#include <locale>
#include <cwctype>
#include <algorithm>
#include <codecvt>
#include <optional>
#include <sstream>
#include <iterator>
#include <map>
#include <type_traits>
#include "Framework.h"

namespace GS
{
#ifdef _UNICODE
	using CHR  = wchar_t;
	using STR  = std::wstring;
	using STRV = std::wstring_view;
	using STRS = std::wstringstream;
#else
	using CHR  = char;
	using STR  = std::string;
	using STRV = std::string_view;
	using STRS = std::stringstream;
#endif

	inline bool IEqual(const STRV& str0, const STRV& str1)
	{
		return std::equal(str0.begin(), str0.end(),
						  str1.begin(), str1.end(),
						  [&](CHR a, CHR b) { return tolower(a) == tolower(b); });
	}

	inline bool StartsWith(const STRV& str, const STRV& prefix)
	{
		return str.find(prefix) == 0;
	}

	inline bool EndsWith(const STRV& str, const STRV& suffix)
	{
		return str.rfind(suffix) == str.size() - suffix.size();
	}
	
	struct TokenSpace : public std::ctype<CHR> 
	{
		TokenSpace(const STRV& chs) 
			: mChars(chs)
		{}
		bool do_is(mask m, char_type ch) const override 
		{
			if (!(m && space))
				return ctype<CHR>::do_is(m, ch);
			return mChars.find(ch) != STRV::npos;
		}
	private:
		const STRV& mChars;
	};

	inline std::vector<STR> Split(const STRV& str, const STRV& chs)
	{
		std::shared_ptr<TokenSpace> spaces = std::make_shared<TokenSpace>(chs);

		STRS ss(STR(str.data()));
		ss.imbue(std::locale(std::locale(), spaces.get()));
		std::istream_iterator<STR, CHR> beg(ss);
		std::istream_iterator<STR, CHR> end;
		
		return std::vector<STR>(beg, end);
	}

	inline STR Trim(const STRV& str)
	{
		static const CHR* _whites = static_cast<const CHR*>(std::is_same_v<CHR, wchar_t>
												   ? (const void*)L" \n\r\t" : (const void*)" \n\r\t");
		size_t offset = 0;
		STR result(str.data());

		offset = result.find_first_not_of(_whites);
		if (offset != STR::npos) result = result.substr(offset);

		offset = result.find_last_not_of(_whites);
		if (offset != STR::npos) result = result.substr(0, offset + 1);

		return result;
	}

	inline STR Replace(const STRV& str, CHR src, CHR dst)
	{
		STR result(str.data());
		std::transform(result.begin(), result.end(), result.begin(),
					   [&](CHR ch) { 
							if (ch == src) 
								return dst;
							else 
								return ch;
					   });
		return result;
	}

	inline STR Replace(const STRV& str, const STRV& src, const STRV& dst)
	{
		STR result(str.data());
		size_t pos = result.find(src.data());

		while (pos != STR::npos)
		{
			result.replace(pos, src.size(), dst.data());
			pos = result.find(src.data(), pos + dst.size());
		}
		return result;
	}

	inline STR Canonicalize(const STRV& path)
	{
		static const STR _kDB = STR(2, (CHR)SLASH);
		STR result = Replace(path, (CHR)SLASH_INV, (CHR)SLASH);

		while (result.find(_kDB) != STR::npos)
			result = Replace(result, _kDB, STR(1, (CHR)SLASH));

		return result;
	}

	inline STR Join(const STRV& delim, 
					const std::vector<STRV>& strs, 
					size_t begin = 0, size_t end = -1)
	{
		STR result;
		size_t i = 0;
		for (auto it = strs.begin(); 
			 it != strs.end() && (end == (size_t)-1 || i < end);
			 it++, i++) 
		{
			result += (*it).data();
			if (it != strs.end() - 1)
				result += delim;
		}
		return result;
	}

	inline STR Join(const CHR& delim,
					const std::vector<STRV>& strs, 
					size_t begin = 0, size_t end = -1)
	{
		return Join(STR(1, delim), strs, begin, end);
	}

	template<typename... ARGS>
	inline STR Join(const STRV& delim, ARGS... args)
	{
		static_assert((std::is_constructible_v<STRV, ARGS&&> && ...));
		std::vector<STRV> vec;
		(vec.push_back(std::forward<ARGS>(args)), ...);
		return Join(delim, vec);
	}

	template<typename... ARGS>
	inline STR JoinPath(ARGS... args)
	{
		return Canonicalize(Join<ARGS...>(STR(1, (CHR)SLASH),
										  std::forward<ARGS>(args)...));
	}

	template<typename T>
	static std::optional<T> GetNumber(const STRV& str, CHR** endPos = nullptr)
	{
		const CHR* _kChars = static_cast<const CHR*>(std::is_same_v<CHR, wchar_t>
												    ? (const void*)L"+-0123456789" : (const void*)"+-0123456789");
		size_t offset = str.find_first_of(_kChars);

		if (offset == STRV::npos)
		{
			if (endPos) *endPos = nullptr;
			return std::nullopt;
		}

		if constexpr (std::is_same_v<CHR, wchar_t>)
		{
			if constexpr (std::is_floating_point_v<T>)
				return std::wcstod((const wchar_t*)str.data() + offset, (wchar_t**)endPos);
			else
				return std::wcstol((const wchar_t*)str.data() + offset, (wchar_t**)endPos, 0);
		}
		else
		{
			if constexpr (std::is_floating_point_v<T>)
				return std::strtod((const char*)str.data() + offset, (char**)endPos);
			else
				return std::strtol((const char*)str.data() + offset, (char**)endPos, 0);
		}
	}

	template<typename T>
	static std::vector<T> GetNumbers(const STRV& str)
	{
		std::vector<T> nums;
		std::optional<T> num;
		CHR* endPos = &str[0];

		while (*endPos != 0)
		{
			num = GetNumber<T>(endPos, &endPos);
			if (num.has_value())
				nums.push_back(num.value());
		}

		return nums;
	}

	static inline std::string WStr2Str(const std::wstring_view& wstr)
	{
		static std::wstring_convert<std::codecvt_utf8<wchar_t>> cvt;
		return cvt.to_bytes(wstr.data());
	}

	static inline std::wstring Str2WStr(const std::string_view& str)
	{
		static std::wstring_convert<std::codecvt_utf8<wchar_t>> cvt;
		return cvt.from_bytes(str.data());
	}

	static inline std::string FromUTF16(const uint16_t* wwstr)
	{
#ifdef _WIN32
		// This is a known issue, tracked by Visual Studio Team
		static std::wstring_convert<std::codecvt_utf8<uint16_t>, uint16_t> cvt;
#else
		static std::wstring_convert<std::codecvt_utf8<char16_t>, char16_t> cvt;
#endif
		return cvt.to_bytes(wwstr);
	}

	static inline std::string FromUTF32(const uint32_t* wwstr)
	{
#ifdef _WIN32
		// This is a known issue, tracked by Visual Studio Team
		static std::wstring_convert<std::codecvt_utf8<uint32_t>, uint32_t> cvt;
#else
		static std::wstring_convert<std::codecvt_utf8<char32_t>, char32_t> cvt;
#endif
		return cvt.to_bytes(wwstr);
	}

}