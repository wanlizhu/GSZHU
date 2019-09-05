#pragma once

#include <string>
#include <vector>
#include <thread>
#include <functional>
#include <optional>
#include "Utils/String.h"

namespace GS
{
	void CheckLastError(const CHR* funcName, bool showBox = false);

	std::vector<fs::path> FindFile(const STRV& filename, const STRV& directory, bool recursive = true);
	GS_API fs::path FindDataFile(const STRV& filename);
	std::vector<fs::path> FindDataFiles(const STRV& filename, bool greed = true);
	bool PathExists(const STRV& path);
	bool FileExists(const STRV& path);
	bool DirectoryExists(const STRV& path);
	GS_API STRV CurrentDirectory();

	std::optional<STR> EnvironmentVariable(const STR& key);

}