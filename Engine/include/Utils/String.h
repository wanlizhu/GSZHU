#pragma once

#include <string>
#include <vector>
#include <locale>
#include <algorithm>
#include <codecvt>
#include <optional>
#include <map>
#include <type_traits>
#include "Framework.h"

namespace GS
{
	template<typename _CH>
	class GS_API SZ
	{
	public:
		using CH  = _CH;
		using STR = std::basic_string<CH, std::char_traits<CH>, std::allocator<CH>>;

		static bool StartsWith(const STR& str, const STR& prefix, bool nocase = false)
		{
			if (str.size() < prefix.size())
				return false;

			if constexpr (nocase)
			{
				STR s = str;
				STR pfx = prefix;
				std::transform(str.begin(), str.end(), s.begin(), ::tolower);
				std::transform(prefix.begin(), prefix.end(), pfx.begin(), ::tolower);
				return s.compare(0, pfx.length(), pfx) == 0;
			}
			else
			{
				return str.compare(0, prefix.length(), prefix) == 0;
			}
		}

		static bool EndsWith(const STR& str, const STR& suffix, bool nocase = false)
		{
			if (str.size() < suffix.size())
				return false;

			STR s = str.substr(str.length() - suffix.length());
			if (nocase)
			{
				STR sfx = suffix;
				std::transform(s.begin(), s.end(), s.begin(), ::tolower);
				std::transform(sfx.begin(), sfx.end(), sfx.begin(), ::tolower);
				return sfx == s;
			}
			else
			{
				return s == suffix;
			}
		}

		static std::vector<STR> Split(const STR& str, CH ch)
		{
			using _ISS = std::basic_istringstream<CH, std::char_traits<CH>, std::allocator<CH>>;

			std::vector<STR> tokens;
			STR token;
			_ISS tokenss(str);
			while (std::getline(tokenss, token, ch)) {
				tokens.push_back(token);
			}

			return tokens;
		}

		static std::vector<STR> Split(const STR& str, const STR& delims)
		{
			std::vector<STR> tokens;
			auto first = std::cbegin(str);

			while (first != std::cend(str))
			{
				const auto second = std::find_first_of(first,
													   std::cend(str),
													   std::cbegin(delims),
													   std::cend(delims));
				if (first != second)
					tokens.emplace_back(first, second);

				if (second == std::cend(str))
					break;

				first = std::next(second);
			}

			return tokens;
		}

		static STR Trim(const STR& str)
		{
			const CH* _whites = static_cast<const CH*>(std::is_same_v<CH, wchar_t> 
													   ? L" \n\r\t" : " \n\r\t");
			size_t offset = 0;
			STR ret;

			offset = str.find_first_not_of(_whites);
			if (offset != STR::npos)
				ret = str.substr(offset);

			offset = ret.find_last_not_of(_whites);
			if (offset != STR::npos)
				ret = ret.substr(0, offset + 1);

			return ret;
		}

		static STR Replace(const STR& str, CH src, CH dst)
		{
			STR res = str;

			for (size_t i = 0; i < res.size(); i++) 
			{
				if (res[i] == src)
					res[i] = dst;
			}

			return res;
		}

		static STR Replace(const STR& str, const STR& src, const STR& dst)
		{
			STR res = str;
			size_t offset = res.find(src);

			while (offset != STR::npos)
			{
				res.replace(offset, src.length(), dst);
				offset += dst.length();
				offset = res.find(src, offset);
			}

			return res;
		}

		static STR Canonicalize(const STR& path)
		{
			static const std::map<int, int> _kUndesiredSlash = {
				{ (int)'\\', (int)'/' },
				{ (int)'/', (int)'\\' },
			};
			static const STR _k2Slashes(2, (CH)PATH_SLASH);

			STR res = Replace(path, (CH)_kUndesiredSlash.at((int)PATH_SLASH), (CH)PATH_SLASH);

			size_t offset = res.find(_k2Slashes);
			while (offset != STR::npos)
			{
				res = res.replace(offset, 2, 1, (CH)PATH_SLASH);
				offset = res.find(_k2Slashes);
			}

			return res;
		}

		static STR Join(const STR& delim, const std::vector<STR>& strs)
		{
			STR res;
			for (auto it = strs.begin(); it != strs.end(); it++)
			{
				res += *it;
				if (it != strs.end() - 1)
					res += delim;
			}
			return res;
		}

		template<typename... ARGS>
		static STR Join(const STR& delim, ARGS... args) 
		{
			static_assert((std::is_constructible_v<STR, ARGS&&> && ...));
			std::vector<STR> vec;
			(vec.push_back(std::forward<ARGS>(args)), ...);
			return Join(delim, vec);
		}

		template<typename... ARGS>
		static STR JoinPath(ARGS... args)
		{
			return SZ<wchar_t>::Canonicalize(Join<ARGS...>(STR(1, (CH)PATH_SLASH),
												  std::forward<ARGS>(args)...));
		}

		template<typename T>
		static std::optional<T> GetNumber(const STR& str, CH** endPos = nullptr)
		{
			const CH* _kChars = static_cast<const CH*>(std::is_same_v<CH, wchar_t>
													   ? (const void*)L"+-0123456789" : (const void*)"+-0123456789");
			size_t offset = str.find_first_of(_kChars);
			
			if (offset == STR::npos)
			{
				if (endPos)
					*endPos = nullptr;
				return std::nullopt;
			}

			if constexpr (std::is_same_v<CH, wchar_t>)
			{
				if constexpr (std::is_floating_point_v<T>)
					return std::wcstod((const wchar_t*)str.c_str() + offset, (wchar_t**)endPos);
				else
					return std::wcstol((const wchar_t*)str.c_str() + offset, (wchar_t**)endPos, 0);
			}
			else
			{
				if constexpr (std::is_floating_point_v<T>)
					return std::strtod((const char*)str.c_str() + offset, (char**)endPos);
				else
					return std::strtol((const char*)str.c_str() + offset, (char**)endPos, 0);
			}
		}

		template<typename T>
		static std::vector<T> GetNumbers(const STR& str)
		{
			std::vector<T> nums;
			std::optional<T> num;
			CH* endPos = &str[0];

			while (*endPos != 0)
			{
				num = GetNumber<T>(endPos, &endPos);
				if (num.has_value())
					nums.push_back(num.value());
			}

			return nums;
		}
		
		static inline std::string WStr2Str(const std::wstring& wstr) 
		{
			static std::wstring_convert<std::codecvt_utf8<wchar_t>> cvt;
			return cvt.to_bytes(wstr);
		}

		static inline std::wstring Str2WStr(const std::string& str)
		{
			static std::wstring_convert<std::codecvt_utf8<wchar_t>> cvt;
			return cvt.from_bytes(str);
		}

		static inline std::string ConvertUTF16(const uint16_t* wwstr)
		{
#ifdef _WIN32
			// This is a known issue, tracked by Visual Studio Team
			static std::wstring_convert<std::codecvt_utf8<uint16_t>, uint16_t> cvt;
#else
			static std::wstring_convert<std::codecvt_utf8<char16_t>, char16_t> cvt;
#endif
			return cvt.to_bytes(wwstr);
		}

		static inline std::string ConvertUTF32(const uint32_t* wwstr)
		{
#ifdef _WIN32
			// This is a known issue, tracked by Visual Studio Team
			static std::wstring_convert<std::codecvt_utf8<uint32_t>, uint32_t> cvt;
#else
			static std::wstring_convert<std::codecvt_utf8<char32_t>, char32_t> cvt;
#endif
			return cvt.to_bytes(wwstr);
		}
	};
}