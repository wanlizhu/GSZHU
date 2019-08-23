#include "Utils/Platform/OS.h"
#include "Utils/String.h"
#include "Framework.h"
#include <Shlwapi.h>
#include <ShlObj.h>
#include <sys/types.h>
#include <Psapi.h>
#include <future>
#include <ShellScalingApi.h>
#include <vector>
#include <filesystem>
#include <stdint.h>
#include <string>

#pragma comment(lib, "Shcore.lib")

// Always run in Optimus mode on laptops
extern "C"
{
	_declspec(dllexport) DWORD NvOptimusEnablement = 1;
}

namespace fs = std::filesystem;

namespace GS
{
	namespace local
	{
		static std::unordered_map<std::string, std::pair<std::thread, bool>> _sDirectoryWatcherThreads;

		static void CheckDirectoryStatus(const std::string& dir,
									     const std::function<void(const std::string&)>& callback,
									     bool watchSubtree)
		{
			HANDLE hFile = CreateFileA(dir.c_str(), 
									   GENERIC_READ | FILE_LIST_DIRECTORY,
									   FILE_SHARE_READ | FILE_SHARE_WRITE,
									   nullptr,
									   OPEN_EXISTING,
									   FILE_FLAG_BACKUP_SEMANTICS | FILE_FLAG_OVERLAPPED,
									   NULL);
			assert(hFile != INVALID_HANDLE_VALUE);

			OVERLAPPED overlapped{};

			while (true)
			{
				size_t offset = 0;
				uint32_t bytesReturned = 0;
				std::vector<uint32_t> buffer;
				buffer.resize(1024);

				if (!ReadDirectoryChangesW(hFile, 
										   buffer.data(),
										   static_cast<uint32_t>(sizeof(uint32_t) * buffer.size()),
										   watchSubtree ? TRUE : FALSE, 
										   FILE_NOTIFY_CHANGE_LAST_WRITE, 
										   0,
										   &overlapped,
										   nullptr))
				{
#if LOG_ENABLED
					LOG(ERROR) << "Failed to read directory changes for shared file.";
#endif
					CloseHandle(hFile);
					return;
				}

				if (!GetOverlappedResult(hFile, &overlapped, (LPDWORD)&bytesReturned, true))
				{
#if LOG_ENABLED
					LOG(ERROR) << "Failed to read directory changes for shared file.";
#endif
					CloseHandle(hFile);
					return;
				}

				// Don't check for another overlapped result if current thread is required to close
				if (!_sDirectoryWatcherThreads.at(dir).second)
				{
					break;
				}

				if (!bytesReturned)
					continue;

				while (offset < buffer.size())
				{
					_FILE_NOTIFY_INFORMATION* pNotifyInfo = reinterpret_cast<_FILE_NOTIFY_INFORMATION*>(buffer.data());
					std::string currentFilename;
					size_t convertedSize = 0;
					currentFilename.resize(pNotifyInfo->FileNameLength, '\0');
					wcstombs_s(&convertedSize,
							   &currentFilename.front(),
							   currentFilename.size(),
							   pNotifyInfo->FileName, 
							   pNotifyInfo->FileNameLength);
					
					if (pNotifyInfo->Action == FILE_ACTION_MODIFIED)
					{
						callback(currentFilename);
						break;
					}

					if (!pNotifyInfo->NextEntryOffset)
						break;

					offset += pNotifyInfo->NextEntryOffset;
				}
			}	

			CloseHandle(hFile);
		}


		template<bool OPEN>
		std::string GetFileDialogFilterString(const std::vector<OS::FileDialogFilter>& filters)
		{
			std::string s;
			std::string d;
			bool appendForOpen = OPEN && filters.size() > 1;
			if (appendForOpen)
				s.append(1, 0);

			for (size_t i = 0; i < filters.size(); i++)
			{
				const auto& f = filters[i];
				if (appendForOpen)
				{
					bool last = i == (filters.size() - 1);
					std::string e = "*." + f.ext;
					if (last == false) e += ';';
					d += e;
					s += e;
				}
				else
				{
					s += f.desc.empty() ? f.ext + " files" : f.desc + " (*." + f.ext + ')';
					s.append(1, 0);
					s += "*." + f.ext + ';';
					s.append(1, 0);
				}
			}

			if (appendForOpen)
				s = "Supported Formats (" + d + ')' + s;

			s.append(appendForOpen ? 2 : 1, 0);

			return s;
		}
	} // namespace local

	OS::EMessageBoxButton OS::ShowMessageBox(const std::string& msg, OS::EMessageBoxType boxtype)
	{
		UINT type = MB_OK;
		switch (boxtype)
		{
		case EMessageBoxType::Ok:
			type = MB_OK;
			break;
		case EMessageBoxType::OkCancel:
			type = MB_OKCANCEL;
			break;
		case EMessageBoxType::RetryCancel:
			type = MB_RETRYCANCEL;
			break;
		case EMessageBoxType::AbortRetryCancel:
			type = MB_ABORTRETRYIGNORE;
			break;
		default:
			assert(false);
		}

		int value = MessageBoxA(nullptr, msg.c_str(), "Message Box", type | MB_TOPMOST);

		switch (value)
		{
		case IDOK:
			return EMessageBoxButton::Ok;
		case IDCANCEL:
			return EMessageBoxButton::Cancel;
		case IDRETRY:
			return EMessageBoxButton::Retry;
		case IDABORT:
			return EMessageBoxButton::Abort;
		case IDIGNORE:
			return EMessageBoxButton::Ignore;
		default:
			assert(false);
			return EMessageBoxButton::Cancel;
		}
	}

	bool OS::FileExists(const std::string& filename)
	{
		DWORD attr = GetFileAttributesA(filename.c_str());
		return (attr != INVALID_FILE_ATTRIBUTES)
			&& !(attr & FILE_ATTRIBUTE_DIRECTORY);
	}

	bool OS::DirectoryExists(const std::string& filename)
	{
		DWORD attr = GetFileAttributesA(filename.c_str());
		return (attr != INVALID_FILE_ATTRIBUTES)
			&& (attr & FILE_ATTRIBUTE_DIRECTORY);
	}

	bool OS::MakeFile(const std::string& filename)
	{
		fs::path path(filename);
		auto parent = path.parent_path();
		if (!OS::DirectoryExists(parent.string()))
		{
			if (!OS::MakeDirectory(parent.string()))
				return false;
		}

		HANDLE hFile = CreateFileA(path.string().c_str(),
					               GENERIC_ALL,
					               0,
					               NULL,
					               CREATE_NEW, // return false if file already exists
					               FILE_ATTRIBUTE_NORMAL,
					               NULL);
		CloseHandle(hFile);
		return hFile != INVALID_HANDLE_VALUE;
	}

	bool OS::MakeDirectory(const std::string& directory)
	{
		std::string str = OS::Canonicalize(directory);
		std::vector<std::string> parts = SZ::Split(str, GetPreferredSeparator() + std::string(""));

		std::string path;
		for (size_t i = 0; i < parts.size(); i++)
		{
			path = SZ::JoinPath(path, parts[i]);
			if (!OS::DirectoryExists(path))
			{
				BOOL res = CreateDirectoryA(path.c_str(), NULL);
				if (res == FALSE)
					return false;
			}
		}

		return true;
	}

	std::string OS::GetTMPDirectory()
	{
		return fs::temp_directory_path().string();
	}

	std::string OS::GetExecutableDirectory()
	{
		static char _exepath[MAX_PATH] = { 0 };

		if (_exepath[0] == 0)
		{
			GetModuleFileNameA(nullptr, _exepath, MAX_PATH);
			int pos = (int)strlen(_exepath);
			while (pos--)
			{
				if (_exepath[pos] == '\\' || _exepath[pos] == '/')
					break;
			}
		}

		return _exepath;
	}

	std::string OS::GetPWD()
	{
		char curdir[MAX_PATH] = { 0 };
		GetCurrentDirectoryA(MAX_PATH, curdir);
		return curdir;
	}

	std::string OS::GetExecutableName()
	{
		static char _exepath[MAX_PATH] = { 0 };
		static const char* _exename = _exepath;

		if (_exename == nullptr)
		{
			GetModuleFileNameA(nullptr, _exepath, MAX_PATH);
			int pos = (int)strlen(_exepath);
			while (pos--)
			{
				if (_exepath[pos] == '\\' || _exepath[pos] == '/')
				{
					_exename = _exepath + pos + 1;
					break;
				}
			}
		}

		return _exename;
	}

	std::optional<std::string> OS::EnvironmentVariable(const std::string& name)
	{
		static char _buffer[4096] = { 0 };
		int size = GetEnvironmentVariableA(name.c_str(), _buffer, arraysize(_buffer));
		assert(size < arraysize(_buffer));
		if (size == 0)
			return std::nullopt;

		return std::string(_buffer);
	}

	template<bool OPEN>
	std::optional<std::string> FileDialog(const std::vector<OS::FileDialogFilter>& filters)
	{
		OPENFILENAMEA ofn;
		CHAR chars[512] = { 0 };
		ZeroMemory(&ofn, sizeof(ofn));

		std::string filtersStr = local::GetFileDialogFilterString<OPEN>(filters);

		ofn.lStructSize = sizeof(OPENFILENAME);
		ofn.hwndOwner = GetForegroundWindow();
		ofn.lpstrFile = (LPSTR)filtersStr.c_str();
		ofn.lpstrFile = chars;
		ofn.nMaxFile = arraysize(chars);
		ofn.Flags = OFN_NOCHANGEDIR | (OPEN ? OFN_FILEMUSTEXIST : 0);
		ofn.lpstrDefExt = "";

		BOOL result = OPEN ? GetOpenFileNameA(&ofn) : GetSaveFileNameA(&ofn);
		if (result)
		{
			std::string str(chars);
			return str;
		}

		return std::nullopt;
	}

	template
	std::optional<std::string> FileDialog<true>(const std::vector<OS::FileDialogFilter>&);
	template
	std::optional<std::string> FileDialog<false>(const std::vector<OS::FileDialogFilter>&);

	bool OS::SetWindowIcon(const std::string& iconFile, WindowHandle handle)
	{
		auto path = OS::FindDataFile(iconFile);
		if (path.has_value())
			return false;

		HANDLE hIcon = LoadImageA(GetModuleHandle(NULL), path.value().c_str(), IMAGE_ICON, 0, 0, LR_DEFAULTSIZE);
		HWND hwnd = handle ? handle : GetActiveWindow();
		SendMessageA(hwnd, WM_SETICON, ICON_BIG, (LPARAM)hIcon);

		return true;
	}

	int OS::GetDisplayDPI()
	{
		::SetProcessDPIAware();
		HDC screen = ::GetDC(NULL);
		double h_pixels_per_inch = ::GetDeviceCaps(screen, LOGPIXELSX);
		double v_pixels_per_inch = ::GetDeviceCaps(screen, LOGPIXELSY);
		::ReleaseDC(NULL, screen);
		
		return int((h_pixels_per_inch + v_pixels_per_inch) * 0.5);
	}

	float OS::GetDisplayScaleFactor()
	{
		float dpi = (float)OS::GetDisplayDPI();
		float scale = dpi / 96.0f;
		return scale;

		::SetProcessDPIAware();
		DEVICE_SCALE_FACTOR factor;
		if (::GetScaleFactorForMonitor(nullptr, &factor) == S_OK)
		{
			switch (factor)
			{
			case SCALE_100_PERCENT: return 1.0f;
			case SCALE_120_PERCENT: return 1.2f;
			case SCALE_125_PERCENT: return 1.25f;
			case SCALE_140_PERCENT: return 1.40f;
			case SCALE_150_PERCENT: return 1.50f;
			case SCALE_160_PERCENT: return 1.60f;
			case SCALE_175_PERCENT: return 1.70f;
			case SCALE_180_PERCENT: return 1.80f;
			case SCALE_200_PERCENT: return 2.00f;
			case SCALE_225_PERCENT: return 2.25f;
			case SCALE_250_PERCENT: return 2.50f;
			case SCALE_300_PERCENT: return 3.00f;
			case SCALE_350_PERCENT: return 3.50f;
			case SCALE_400_PERCENT: return 4.00f;
			case SCALE_450_PERCENT: return 4.50f;
			case SCALE_500_PERCENT: return 4.60f;
			default:
				assert(false);
				return 1.0f;
			}
		}
		return 1.0f;
	}

	bool OS::IsDebuggerPresent()
	{
#ifdef _DEBUG
		return ::IsDebuggerPresent();
#else
		return false;
#endif
	}

	void OS::DebugOutput(const std::string& msg)
	{
		OutputDebugStringA(msg.c_str());
	}

	void OS::DebugBreak()
	{
		__debugbreak();
	}

	size_t OS::ExecuteProcess(const std::string& appname, const std::string& args)
	{
		std::string commandLine = appname + " " + args;
		STARTUPINFOA startup{};
		PROCESS_INFORMATION info{};
		
		if (!CreateProcessA(nullptr,
			                (LPSTR)commandLine.c_str(),
							nullptr,
							nullptr,
							TRUE,
							NORMAL_PRIORITY_CLASS,
							nullptr,
							nullptr,
							&startup,
							&info))
		{
#if LOG_ENABLED
			LOG(ERROR) << "Unable to start a process.";
			return 0;
#endif
		}
		return reinterpret_cast<size_t>(info.hProcess);
	}

	bool OS::IsProcessActive(size_t id)
	{
		uint32_t code = 0;
		if (GetExitCodeProcess((HANDLE)id, (LPDWORD)& code))
		{
			if (code != STILL_ACTIVE)
				return false;
		}
		return true;
	}

	void OS::TerminateProcess(size_t id)
	{
		::TerminateProcess((HANDLE)id, 0);
		CloseHandle((HANDLE)id);
	}

	void OS::StartDirectoryWatcher(const std::string& dir,
						     	   const std::function<void(const std::string&)>& callback,
						     	   bool watchSubtree)
	{
		const auto& it = local::_sDirectoryWatcherThreads.find(dir);
		// Only have one thread waiting on directory modification
		if (it != local::_sDirectoryWatcherThreads.end())
		{
			if (it->second.first.joinable())
				it->second.first.join();
		}

		local::_sDirectoryWatcherThreads[dir].first = std::thread(local::CheckDirectoryStatus,
																	 dir,
																	 callback,
																	 watchSubtree);
		local::_sDirectoryWatcherThreads[dir].second = true;
	}

	void OS::StopDirectoryWatcher(const std::string& dir)
	{
		const auto& it = local::_sDirectoryWatcherThreads.find(dir);
		// Only have one thread waiting on directory modification
		if (it != local::_sDirectoryWatcherThreads.end())
		{
			it->second.second = false;
			it->second.first.detach();
		}
	}

	std::optional<std::string> OS::FindFile(const std::string& filename,
											const std::string& directory,
											bool findSubtree,
											bool includeDir)
	{
		auto vec = OS::FindFiles(filename, directory, findSubtree, includeDir, true);
		if (!vec || vec->size() == 0)
		{
			return std::nullopt;
		}
		return vec->at(0);
	}

	std::shared_ptr<std::vector<std::string>> OS::FindFiles(const std::string& filename,
															const std::string& directory,
															bool findSubtree,
															bool includeDir,
															bool onlyFirst)
	{
		WIN32_FIND_DATAA fd{};
		HANDLE hFind = INVALID_HANDLE_VALUE;

		std::string filter = SZ::JoinPath(directory, filename);
		char szFilter[MAX_PATH] = { 0 };
		strcpy_s(szFilter, filter.size() + 1, filter.c_str());

		hFind = FindFirstFileA(szFilter, &fd);
		if (hFind == INVALID_HANDLE_VALUE)
		{
			return nullptr;
		}

		auto vec = std::make_shared<std::vector<std::string>>();
		std::vector<std::string> subdirs;
		do
		{
			if (strcmp(fd.cFileName, ".") == 0 ||
				strcmp(fd.cFileName, "..") == 0)
				continue;

			std::string filepath = OS::Canonicalize(SZ::JoinPath(directory, fd.cFileName));

			bool isDir = (fd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) != 0;
			if (isDir && findSubtree)
				subdirs.push_back(filepath);

			if (isDir && !includeDir)
				continue;

			vec->push_back(filepath);
		} while (FindNextFileA(hFind, &fd) != 0);

		if (findSubtree)
		{
			for (auto subdir : subdirs)
			{
				auto subvec = OS::FindFiles(filename, 
											subdir, 
											findSubtree,
											includeDir,
											onlyFirst);

				if (!subvec)
					continue;

				vec->insert(vec->end(), subvec->begin(), subvec->end());
			}
		}

		return vec->empty() ? nullptr : vec;
	}

	std::thread::native_handle_type OS::GetCurrentThread()
	{
		return ::GetCurrentThread();
	}

	void OS::SetThreadAffinity(std::thread::native_handle_type thread, uint32_t mask)
	{
		::SetThreadAffinityMask(thread, mask);
		if (DWORD error = GetLastError() != 0)
		{
#if LOG_ENABLED
			LPVOID buffer;
			FormatMessageA(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
						  NULL, 
						  error,
						  MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
						  (LPTSTR)&buffer,
						  0,
						  NULL);
			std::string errorMessage((LPTSTR)buffer);
			LOG(ERROR) << "SetThreadAffinity() failed: " << errorMessage;
			LocalFree(buffer);
#endif
		}
	}

	void OS::SetThreadPriority(std::thread::native_handle_type thread, OS::EThreadPriority priority)
	{
		if (priority >= EThreadPriority::Lowest)
			::SetThreadPriority(thread, THREAD_BASE_PRIORITY_MIN + (int32_t)priority);
		else if (priority == EThreadPriority::BackgroundBegin)
			::SetThreadPriority(thread, THREAD_MODE_BACKGROUND_BEGIN);
		else if (priority == EThreadPriority::BackgroundEnd)
			::SetThreadPriority(thread, THREAD_MODE_BACKGROUND_END);
		else
			assert(false);

		if (DWORD error = GetLastError() != 0)
		{
#if LOG_ENABLED
			LPVOID buffer;
			FormatMessageA(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
						  NULL,
						  error,
						  MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
						  (LPTSTR)&buffer,
						  0,
						  NULL);
			std::string errorMessage((LPTSTR)buffer);
			LOG(ERROR) << "SetThreadPriority() failed: " << errorMessage;
			LocalFree(buffer);
#endif
		}
	}

	time_t OS::GetFileModifiedTime(const std::string& filename)
	{
		struct stat s;
		if (stat(filename.c_str(), &s) != 0)
		{
#if LOG_ENABLED
			LOG(ERROR) << "Can't get file time for '" << filename << "'";
#endif
			return 0;
		}

		return s.st_mtime;
	}

	uint64_t OS::GetTotalVirtualMemory()
	{
		MEMORYSTATUSEX memInfo;
		memInfo.dwLength = sizeof(MEMORYSTATUSEX);
		GlobalMemoryStatusEx(&memInfo);
		DWORDLONG totalVirtualMem = memInfo.ullTotalPageFile;

		return totalVirtualMem;
	}

	uint64_t OS::GetUsedVirtualMemory()
	{
		MEMORYSTATUSEX memInfo;
		memInfo.dwLength = sizeof(MEMORYSTATUSEX);
		GlobalMemoryStatusEx(&memInfo);
		DWORDLONG totalVirtualMem = memInfo.ullTotalPageFile;
		DWORDLONG virtualMemUsed = memInfo.ullTotalPageFile - memInfo.ullAvailPageFile;

		return virtualMemUsed;
	}

	uint64_t OS::GetProcessUsedVirtualMemory()
	{
		PROCESS_MEMORY_COUNTERS_EX pmc;
		GetProcessMemoryInfo(GetCurrentProcess(), (PROCESS_MEMORY_COUNTERS*)& pmc, sizeof(pmc));
		SIZE_T virtualMemUsedByMe = pmc.PrivateUsage;

		return virtualMemUsedByMe;
	}

	SharedLibraryHandle OS::LoadSharedLibrary(const std::string& path)
	{
		return ::LoadLibraryA(path.c_str());
	}

	void OS::FreeSharedLibrary(SharedLibraryHandle handle)
	{
		::FreeLibrary(handle);
	}

	void* OS::GetProcAddress(SharedLibraryHandle handle, const std::string& func)
	{
		return ::GetProcAddress(handle, func.c_str());
	}
}