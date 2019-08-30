#include "Utils/Platform/OS.h"
#include "Utils/String.h"
#include <fstream>
#include <filesystem>
#include <regex>

namespace fs = std::filesystem;

namespace GS
{
	static std::vector<std::wstring> _sDataDirectories =
	{
		OS::GetPWD(),
		SZ<wchar_t>::Canonicalize(OS::GetPWD() + L"/data"),
		SZ<wchar_t>::Canonicalize(OS::GetPWD() + L"/resource"),
		SZ<wchar_t>::Canonicalize(OS::GetPWD() + L"/assets"),
		SZ<wchar_t>::Canonicalize(OS::GetPWD() + L"/media"),
	};

	template<bool OPEN>
	std::optional<std::wstring> FileDialog(const std::vector<OS::FileDialogFilter>&);


	std::optional<fs::path> OS::OpenFileDialog(const std::vector<OS::FileDialogFilter>& filters)
	{
		return FileDialog<true>(filters);
	}

	std::optional<fs::path> OS::SaveFileDialog(const std::vector<OS::FileDialogFilter>& filters)
	{
		return FileDialog<false>(filters);
	}

	const std::vector<std::wstring>& OS::GetDataDirectories()
	{
		return _sDataDirectories;
	}

	void OS::AddDataDirectory(const std::wstring& directory)
	{
		auto it = std::find_if(_sDataDirectories.begin(), _sDataDirectories.end(),
							   [&](const std::wstring& str) {
								   return SZ<wchar_t>::Canonicalize(directory) == str;
							   });
		if (it == _sDataDirectories.end())
		{
			_sDataDirectories.push_back(SZ<wchar_t>::Canonicalize(directory));
		}
	}

	void OS::RemoveDataDirectory(const std::wstring& directory)
	{
		auto it = std::find_if(_sDataDirectories.begin(), _sDataDirectories.end(),
							   [&](const std::wstring& str) {
								   return SZ<wchar_t>::Canonicalize(directory) == str;
							   });
		if (it != _sDataDirectories.end())
		{
			_sDataDirectories.erase(it);
		}
	}

	bool OS::IsAbsolutePath(const std::wstring& filename)
	{
		return fs::path(filename).is_absolute();
	}

	std::optional<fs::path> OS::FindDataFile(const std::wstring& filename)
	{
		static bool _inited = false;
		if (!_inited)
		{
			_inited = true;
			std::optional<std::wstring> value = OS::EnvironmentVariable(L"GS_DATA_PATH");
			if (value)
			{
				auto dirs = SZ<wchar_t>::Split(value.value(), L";");
				_sDataDirectories.insert(_sDataDirectories.end(), dirs.begin(), dirs.end());
			}
		}

		// Check if it's already an absolute path
		if (OS::FileExists(filename))
			return SZ<wchar_t>::Canonicalize(filename);

		for (const auto dir : _sDataDirectories)
		{
			auto path = OS::FindFile(filename, dir);
			if (path.has_value())
				return path;
		}

		return std::nullopt;
	}

	std::wstring OS::FindAvailableFileName(const std::wstring& basename,
					  				       const std::wstring& ext,
					  				       const std::wstring& directory)
	{
		std::wstring path = SZ<wchar_t>::Canonicalize(directory + L"/" + basename + ext);
		if (OS::FileExists(path))
		{
			const std::wregex rx(L"");
			std::wsmatch match;
			if (std::regex_match(basename, match, rx))
			{
				std::wstring suffix = match[match.size() - 1].str();
				int id = SZ<wchar_t>::GetNumber<int>(suffix).value();
				std::wstring stem = basename.substr(0, basename.size() - suffix.size());

				path = SZ<wchar_t>::Canonicalize(directory + L"/" + stem + L" (" + std::to_wstring(++id) + L")" + ext);
				while (OS::FileExists(path))
				{
					path = SZ<wchar_t>::Canonicalize(directory + L"/" + stem + L" (" + std::to_wstring(++id) + L")" + ext);
				}
			}
			else
			{
				return SZ<wchar_t>::Canonicalize(directory + L"/" + basename + L" (1)" + ext);
			}
		}
		
		return path;
	}

	std::wstring OS::GetDirectory(const std::wstring& filename)
	{
		fs::path path(filename);
		return path.has_parent_path() ? path.parent_path().wstring() : filename;
	}

	std::wstring OS::GetExtension(const std::wstring& filename)
	{
		fs::path path(filename);
		return path.has_extension() ? path.extension().wstring() : L"";
	}

	std::wstring OS::GetFileName(const std::wstring& filename)
	{
		return fs::path(filename).filename().wstring();
	}

	std::wstring OS::GetStemName(const std::wstring& filename)
	{
		return fs::path(filename).stem().wstring();
	}

	std::shared_ptr<std::string> OS::LoadFile(const std::wstring& filename)
	{
		std::ifstream fstream;
		fstream.open(filename, std::ios::in);
		if (!fstream.is_open())
			return 0;

		fstream.seekg(0, std::ios::end);
		auto data = std::make_shared<std::string>();
		data->reserve(fstream.tellg());

		fstream.seekg(0, std::ios::beg);
		data->assign(std::istreambuf_iterator<char>(fstream),
					 std::istreambuf_iterator<char>());
		fstream.close();

		return data;
	}

	size_t OS::SaveFile(const std::wstring& filename, const void* data, size_t size, bool append)
	{
		std::ofstream fstream;
		fstream.open(filename, append ? std::ios::app : ( std::ios::out | std::ios::trunc));
		if (!fstream.is_open())
			return 0;

		fstream.write((const char*)data, size);
		fstream.close();

		return size;
	}
}