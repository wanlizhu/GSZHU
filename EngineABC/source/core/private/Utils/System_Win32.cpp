#include "Utils/System.h"
#include <queue>
#include <strsafe.h>

namespace GS
{
	static std::vector<STR> _sDataDirectories =
	{
		STR(CurrentDirectory().data()),
		Canonicalize(STR(CurrentDirectory().data()) + TEXT("/data")),
		Canonicalize(STR(CurrentDirectory().data()) + TEXT("/resource")),
		Canonicalize(STR(CurrentDirectory().data()) + TEXT("/assets")),
		Canonicalize(STR(CurrentDirectory().data()) + TEXT("/media")),
	};

	namespace internal
	{
		template<typename TP>
		TP FileTime2TimePoint(const FILETIME& ft)
		{
			SYSTEMTIME st{ 0 };
			if (!FileTimeToSystemTime(&ft, &st))
			{
#if LOG_ENABLED
				LOG(ERROR) << "Invalid FILETIME.";
#endif
				return _tpoint::min();
			}

			ULARGE_INTEGER ul;
			ul.LowPart = ft.dwLowDateTime;
			ul.HighPart = ft.dwHighDateTime;

			std::time_t secs = ul.QuadPart / 10000000ULL - 11644473600ULL;
			std::chrono::milliseconds ms((ul.QuadPart / 10000ULL) % 1000);

			auto tp = std::chrono::system_clock::from_time_t(secs);
			tp += ms;

			return duration_cast<TP>(tp);
		}
	} // namespace internal


	void CheckLastError(const CHR* funcName, bool showBox)
	{
		LPVOID message = nullptr;
		LPVOID display = nullptr;
		DWORD err = ::GetLastError();

		::FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER |
						FORMAT_MESSAGE_FROM_SYSTEM |
						FORMAT_MESSAGE_IGNORE_INSERTS,
						NULL,
						err,
						MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
						(LPTSTR)& message,
						0,
						NULL);
		display = (LPVOID)::LocalAlloc(LMEM_ZEROINIT,
			(lstrlen((LPCTSTR)message) + lstrlen((LPCTSTR)funcName) + 40) * sizeof(TCHAR));

		StringCchPrintf((LPTSTR)display,
						::LocalSize(display) / sizeof(TCHAR),
						TEXT("%s failed with error %d: %s"),
						funcName,
						err,
						message);
		if (showBox)
			MessageBox(NULL, (LPCTSTR)display, TEXT("Error"), MB_OK);

#if LOG_ENABLED
#ifdef _UNICODE
		LOG(ERROR) << WStr2Str((LPCWSTR)display);
#else
		LOG(ERROR) << (LPCSTR)display;
#endif
#endif
		LocalFree(message);
		LocalFree(display);
	}

	std::vector<fs::path> FindFile(const STRV& filename, const STRV& directory, bool recursive)
	{
		static auto SearchDirectoryFunc = [](const STRV& name, const STRV& dir,
											  std::vector<fs::path>* results,
											  std::queue<fs::path>* queue) {
			WIN32_FIND_DATA fd{};
			HANDLE hFind = FindFirstFile(name.data(), &fd);
			if (hFind == INVALID_HANDLE_VALUE)
				return;
			do
			{
				if (lstrcmp(fd.cFileName, TEXT(".")) == 0 ||
					lstrcmp(fd.cFileName, TEXT("..")) == 0)
					continue;

				results->push_back(JoinPath(dir, fd.cFileName));
				if (queue && (fd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY))
					queue->push(JoinPath(dir, fd.cFileName));
			} while (FindNextFile(hFind, &fd));
		};

		std::queue<fs::path> queue;
		queue.push(directory);
		auto nameParts = Split(filename, TEXT("/\\"));
		STRV nameReal = *(nameParts.end() - 1);
		STRV namePrefix = Join((CHR)SLASH, nameParts, 0, nameParts.size() - 1);
		std::vector<fs::path> results;

		while (!queue.empty())
		{
			STR current = Canonicalize(queue.front().wstring());
			queue.pop();

			SearchDirectoryFunc(nameReal, current, &results, &queue);
		}

		if (filename.find_first_of(TEXT("/\\")) != STRV::npos)
		{
			for (auto it = results.begin(); it != results.end();)
			{
				if (!EndsWith(it->parent_path().wstring(), namePrefix))
					it = results.erase(it);
				else
					it++;
			}
		}

		return results;
	}

	fs::path FindDataFile(const STRV& filename)
	{
		auto files = FindDataFiles(filename, false);
		return files.empty() ? fs::path() : files[0];
	}

	std::vector<fs::path> FindDataFiles(const STRV& filename, bool greed)
	{
		static bool _inited = false;
		if (!_inited)
		{
			_inited = true;
			std::optional<STR> value = EnvironmentVariable(TEXT("GS_DATA_PATH"));
			if (value.has_value())
			{
				auto paths = Split(value.value(), TEXT(";"));
				_sDataDirectories.insert(_sDataDirectories.end(), paths.begin(), paths.end());
			}
		}

		if (FileExists(filename))
			return { Canonicalize(filename) };

		std::vector<fs::path> results;
		for (const auto& directory : _sDataDirectories)
		{
			auto paths = FindFile(filename, directory, true);
			results.insert(results.end(), paths.begin(), paths.end());

			if (!results.empty() && !greed)
				break;
		}
		return results;
	}

	bool PathExists(const STRV& path)
	{
		DWORD attr = GetFileAttributes(path.data());
		return (attr != INVALID_FILE_ATTRIBUTES);
	}

	bool FileExists(const STRV& path)
	{
		DWORD attr = GetFileAttributes(path.data());
		return (attr != INVALID_FILE_ATTRIBUTES)
			&& !(attr & FILE_ATTRIBUTE_DIRECTORY);
	}

	bool DirectoryExists(const STRV& path)
	{
		DWORD attr = GetFileAttributes(path.data());
		return (attr != INVALID_FILE_ATTRIBUTES)
			&& (attr & FILE_ATTRIBUTE_DIRECTORY);
	}

	STRV CurrentDirectory()
	{
		static TCHAR _buffer[MAX_PATH] = { 0 };
		if (_buffer[0] == TCHAR(0))
		{
			GetCurrentDirectory(MAX_PATH, _buffer);
		}
		return _buffer;
	}

	std::optional<STR> EnvironmentVariable(const STR& key)
	{
		static TCHAR _buffer[4096] = { 0 };
		int size = GetEnvironmentVariable(key.data(), _buffer, arraysize(_buffer));
		assert(size < arraysize(_buffer));
		if (size == 0)
			return std::nullopt;

		return _buffer;
	}
}