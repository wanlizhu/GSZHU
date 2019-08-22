#include "Utils/Platform/OS.h"
#include "Utils/String.h"
#include <fstream>
#include <filesystem>
#include <regex>

namespace fs = std::filesystem;

namespace GS
{
	std::vector<std::string> _gDataDirectories =
	{
		OS::GetPWD(),
		OS::Canonicalize(OS::GetPWD() + "/data"),
		OS::Canonicalize(OS::GetPWD() + "/assets"),
		OS::Canonicalize(OS::GetPWD() + "/media"),
	};

	template<bool OPEN>
	bool FileDialog(const std::vector<OS::FileDialogFilter>&, std::string*);

	bool OS::OpenFileDialog(const std::vector<OS::FileDialogFilter>& filters, std::string* filename)
	{
		return FileDialog<true>(filters, filename);
	}

	bool OS::SaveFileDialog(const std::vector<OS::FileDialogFilter>& filters, std::string* filename)
	{
		return FileDialog<false>(filters, filename);
	}

	const std::vector<std::string>& OS::GetDataDirectories()
	{
		return _gDataDirectories;
	}

	void OS::AddDataDirectory(const std::string& directory)
	{
		auto it = std::find_if(_gDataDirectories.begin(), _gDataDirectories.end(), 
							   [&](const std::string& str) {
								   return OS::Canonicalize(directory) == str;
							   });
		if (it == _gDataDirectories.end())
		{
			_gDataDirectories.push_back(OS::Canonicalize(directory));
		}
	}

	void OS::RemoveDataDirectory(const std::string& directory)
	{
		auto it = std::find_if(_gDataDirectories.begin(), _gDataDirectories.end(),
							   [&](const std::string& str) {
								   return OS::Canonicalize(directory) == str;
							   });
		if (it != _gDataDirectories.end())
		{
			_gDataDirectories.erase(it);
		}
	}

	std::string OS::Canonicalize(const std::string& filename)
	{
#ifdef _WIN32
#define NORM "\\"
#define INV "/"
#else
#define NORM "/"
#define INV "\\"
#endif
		fs::path path(SZ::Replace(filename, INV, NORM));
		return fs::canonical(path).string();
	}

	bool OS::IsAbsolutePath(const std::string& filename)
	{
		return fs::path(filename).is_absolute();
	}

	std::string OS::FindDataFile(const std::string& filename)
	{
		static bool _inited = false;
		if (!_inited)
		{
			_inited = true;
			std::string value;
			if (OS::ReadEnvironmentVariable("GS_DATA_PATH", &value))
			{
				auto dirs = SZ::Split(value, ";");
				_gDataDirectories.insert(_gDataDirectories.end(), dirs.begin(), dirs.end());
			}
		}

		if (OS::FileExists(filename))
			return OS::Canonicalize(filename);

		for (const auto dir : _gDataDirectories)
		{
			auto path = OS::FindFile(filename, dir);
			if (!path.empty())
				return path;
		}

		return "";
	}

	std::string OS::FindAvailableFileName(const std::string& basename,
					  				      const std::string& ext,
					  				      const std::string& directory)
	{
		std::string path = OS::Canonicalize(directory + "/" + basename + ext);
		if (OS::FileExists(path))
		{
			const std::regex rx("");
			std::smatch match;
			if (std::regex_match(basename, match, rx))
			{
				std::string suffix = match[match.size() - 1].str();
				int id = SZ::GetNumber<int>(suffix);
				std::string stem = basename.substr(0, basename.size() - suffix.size());

				path = OS::Canonicalize(directory + "/" + stem + " (" + std::to_string(++id) + ")" + ext);
				while (OS::FileExists(path))
				{
					path = OS::Canonicalize(directory + "/" + stem + " (" + std::to_string(++id) + ")" + ext);
				}
			}
			else
			{
				return OS::Canonicalize(directory + "/" + basename + " (1)" + ext);
			}
		}
		
		return path;
	}
}