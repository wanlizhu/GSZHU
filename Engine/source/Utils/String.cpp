#include "Utils/String.h"
#include <regex>

namespace GS
{
	bool SZ::StartsWith(const std::string& str, const std::string& prefix, bool nocase)
	{
		if (str.size() < prefix.size())
			return false;

		if (nocase)
		{
			std::string s = str;
			std::string pfx = prefix;
			std::transform(str.begin(), str.end(), s.begin(), ::tolower);
			std::transform(prefix.begin(), prefix.end(), pfx.begin(), ::tolower);
			return s.compare(0, pfx.length(), pfx) == 0;
		}
		else
		{
			return str.compare(0, prefix.length(), prefix) == 0;
		}
	}

	bool SZ::EndsWith(const std::string& str, const std::string& suffix, bool nocase)
	{
		if (str.size() < suffix.size())
			return false;

		std::string s = str.substr(str.length() - suffix.length());
		if (nocase)
		{
			std::string sfx = suffix;
			std::transform(s.begin(), s.end(), s.begin(), ::tolower);
			std::transform(sfx.begin(), sfx.end(), sfx.begin(), ::tolower);
			return sfx == s;
		}
		else
		{
			return s == suffix;
		}
	}

	std::vector<std::string> SZ::Split(const std::string& str, const std::string& delims)
	{
		std::string s;
		std::vector<std::string> vec;

		for (char c : str)
		{
			if (delims.find(c) != std::string::npos)
			{
				if (s.length())
				{
					vec.push_back(s);
					s.clear();
				}
			}
			else
			{
				s += c;
			}
		}

		if (s.length())
		{
			vec.push_back(s);
		}

		return vec;
	}

	std::string SZ::Trim(const std::string& str)
	{
		size_t offset = 0;
		std::string ret;

		offset = str.find_first_not_of(" \n\r\t");
		if (offset != std::string::npos)
			ret = str.substr(offset);

		offset = ret.find_last_not_of(" \n\r\t");
		if (offset != std::string::npos)
			ret = ret.substr(0, offset + 1);
		
		return ret;
	}

	std::string SZ::Replace(const std::string& str, const std::string& src, const std::string& dst)
	{
		std::string res = str;
		size_t offset = res.find(src);

		while (offset != std::string::npos)
		{
			res.replace(offset, src.length(), dst);
			offset += dst.length();
			offset = res.find(src, offset);
		}

		return res;
	}

	std::string SZ::Join(const std::string& delim, const std::vector<std::string>& strs)
	{
		std::string res;
		for (auto it = strs.begin(); it != strs.end(); it++)
		{
			res += *it;
			if (it != strs.end() - 1)
				res += delim;
		}
		return res;
	}

}