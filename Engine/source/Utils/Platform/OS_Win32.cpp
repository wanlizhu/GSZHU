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
#include <strsafe.h>

#pragma comment(lib, "Shcore.lib")

// Always run in Optimus mode on laptops
extern "C"
{
	_declspec(dllexport) DWORD NvOptimusEnablement = 1;
}

namespace GS
{
	namespace local
	{
		static std::unordered_map<std::wstring, std::pair<std::thread, bool>> _sDirectoryWatcherThreads;

		static void CheckDirectoryStatus(const std::wstring& dir,
									     const std::function<void(const std::wstring&)>& callback,
									     bool watchSubtree)
		{
			HANDLE hFile = CreateFile(dir.c_str(), 
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
					FILE_NOTIFY_INFORMATION* pNotifyInfo = reinterpret_cast<FILE_NOTIFY_INFORMATION*>(buffer.data());
					
					if (pNotifyInfo->Action == FILE_ACTION_MODIFIED)
					{
						callback(pNotifyInfo->FileName);
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
		std::wstring GetFileDialogFilterString(const std::vector<OS::FileDialogFilter>& filters)
		{
			std::wstring s;
			std::wstring d;
			bool appendForOpen = OPEN && filters.size() > 1;
			if (appendForOpen)
				s.append(1, 0);

			for (size_t i = 0; i < filters.size(); i++)
			{
				const auto& f = filters[i];
				if (appendForOpen)
				{
					bool last = i == (filters.size() - 1);
					std::wstring e = "*." + f.ext;
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


		constexpr size_t MakeUInt64(DWORD high, DWORD low)
		{
			ULARGE_INTEGER ul{ 0 };
			ul.HighPart = high;
			ul.LowPart = low;
			return size_t(ul.QuadPart);
		}

		constexpr std::time_t FileTime2TimeT(const FILETIME& ft)
		{
			ULARGE_INTEGER ul{ 0 };
			ul.HighPart = ft.dwHighDateTime;
			ul.LowPart = ft.dwLowDateTime;
			return ul.QuadPart / 10000000ULL - 11644473600ULL;
		}

		template<typename _tpoint>
		_tpoint FileTime2TimePoint(const FILETIME& ft)
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
			std::chrono::milliseconds ms((ull.QuadPart / 10000ULL) % 1000);

			auto tp = std::chrono::system_clock::from_time_t(secs);
			tp += ms;

			return duration_cast<_tpoint>(tp);
		}

		void CheckLastError(const char* funcName, bool showBox = false)
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
							(LPTSTR)&message,
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
			std::string str;
			if constexpr (std::is_same_v<TCHAR, char>)
				str = SZ<char>::WStr2Str((LPCWSTR)display);
			else
				str = std::string((LPCSTR)display);
			LOG(ERROR) << str;
#endif
			LocalFree(message);
			LocalFree(display);
		}
	} // namespace local

	OS::EMessageBoxButton OS::ShowMessageBox(const std::wstring& msg, OS::EMessageBoxType boxtype)
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

		int value = MessageBox(nullptr, msg.c_str(), L"Message Box", type | MB_TOPMOST);

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

	bool OS::FileExists(const std::wstring& filename)
	{
		DWORD attr = GetFileAttributes(filename.c_str());
		return (attr != INVALID_FILE_ATTRIBUTES)
			&& !(attr & FILE_ATTRIBUTE_DIRECTORY);
	}

	bool OS::DirectoryExists(const std::wstring& filename)
	{
		DWORD attr = GetFileAttributes(filename.c_str());
		return (attr != INVALID_FILE_ATTRIBUTES)
			&& (attr & FILE_ATTRIBUTE_DIRECTORY);
	}

	bool OS::MakeFile(const std::wstring& filename)
	{
		fs::path path(filename);
		auto parent = path.parent_path();
		if (!OS::DirectoryExists(parent.wstring()))
		{
			if (!OS::MakeDirectory(parent.wstring()))
				return false;
		}

		HANDLE hFile = CreateFile(path.wstring().c_str(),
					              GENERIC_ALL,
					              0,
					              NULL,
					              CREATE_NEW, // return false if file already exists
					              FILE_ATTRIBUTE_NORMAL,
					              NULL);
		CloseHandle(hFile);
		return hFile != INVALID_HANDLE_VALUE;
	}

	bool OS::MakeDirectory(const std::wstring& directory)
	{
		std::wstring str = SZ<wchar_t>::Canonicalize(directory);
		std::vector<std::wstring> parts = SZ<wchar_t>::Split(str, (wchar_t)PATH_SLASH);

		std::wstring path;
		for (size_t i = 0; i < parts.size(); i++)
		{
			path = SZ<wchar_t>::JoinPath(path, parts[i]);
			if (!OS::DirectoryExists(path))
			{
				BOOL res = CreateDirectory(path.c_str(), NULL);
				if (res == FALSE)
					return false;
			}
		}

		return true;
	}

	std::wstring OS::GetTMPDirectory()
	{
		return fs::temp_directory_path().wstring();
	}

	std::wstring OS::GetExecutableDirectory()
	{
		static wchar_t _exePath[MAX_PATH] = { 0 };

		if (_exePath[0] == 0)
		{
			GetModuleFileName(nullptr, _exePath, MAX_PATH);
			int pos = (int)wcslen(_exePath);
			while (pos--)
			{
				if (_exePath[pos] == L'\\' || _exePath[pos] == L'/')
					break;
			}
		}

		return _exePath;
	}

	std::wstring OS::GetPWD()
	{
		wchar_t _curdir[MAX_PATH] = { 0 };
		GetCurrentDirectory(MAX_PATH, _curdir);
		return _curdir;
	}

	std::wstring OS::GetExecutableName()
	{
		static wchar_t _exePath[MAX_PATH] = { 0 };
		static const wchar_t* _exeName = _exePath;

		if (_exeName == nullptr)
		{
			GetModuleFileName(nullptr, _exePath, MAX_PATH);
			int pos = (int)wcslen(_exePath);
			while (pos--)
			{
				if (_exePath[pos] == '\\' || _exePath[pos] == '/')
				{
					_exeName = _exePath + pos + 1;
					break;
				}
			}
		}

		return _exeName;
	}

	std::optional<std::wstring> OS::EnvironmentVariable(const std::wstring& name)
	{
		static wchar_t _buffer[4096] = { 0 };
		int size = GetEnvironmentVariable(name.c_str(), _buffer, arraysize(_buffer));
		assert(size < arraysize(_buffer));
		if (size == 0)
			return std::nullopt;

		return std::wstring(_buffer);
	}

	template<bool OPEN>
	std::optional<std::wstring> FileDialog(const std::vector<OS::FileDialogFilter>& filters)
	{
		OPENFILENAME ofn;
		TCHAR chars[512] = { 0 };
		ZeroMemory(&ofn, sizeof(ofn));

		std::wstring filtersStr = local::GetFileDialogFilterString<OPEN>(filters);

		ofn.lStructSize = sizeof(OPENFILENAME);
		ofn.hwndOwner = GetForegroundWindow();
		ofn.lpstrFile = (LPWSTR)filtersStr.c_str();
		ofn.lpstrFile = chars;
		ofn.nMaxFile = arraysize(chars);
		ofn.Flags = OFN_NOCHANGEDIR | (OPEN ? OFN_FILEMUSTEXIST : 0);
		ofn.lpstrDefExt = L"";

		BOOL result = OPEN ? GetOpenFileName(&ofn) : GetSaveFileName(&ofn);
		if (result)
		{
			std::wstring str(chars);
			return str;
		}

		return std::nullopt;
	}

	template
	std::optional<std::wstring> FileDialog<true>(const std::vector<OS::FileDialogFilter>&);
	template
	std::optional<std::wstring> FileDialog<false>(const std::vector<OS::FileDialogFilter>&);

	bool OS::SetWindowIcon(const std::wstring& iconFile, WindowHandle handle)
	{
		auto path = OS::FindDataFile(iconFile);
		if (path.has_value())
			return false;

		HANDLE hIcon = LoadImage(GetModuleHandle(NULL), path.value().c_str(), IMAGE_ICON, 0, 0, LR_DEFAULTSIZE);
		HWND hwnd = handle ? handle : GetActiveWindow();
		SendMessage(hwnd, WM_SETICON, ICON_BIG, (LPARAM)hIcon);

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

	void OS::DebugOutput(const std::wstring& msg)
	{
		OutputDebugString(msg.c_str());
	}

	void OS::DebugBreak()
	{
		__debugbreak();
	}

	size_t OS::ExecuteProcess(const std::wstring& appname, const std::wstring& args)
	{
		std::wstring commandLine = appname + L" " + args;
		STARTUPINFO startup{};
		PROCESS_INFORMATION info{};
		
		if (!CreateProcess(nullptr,
			               (LPWSTR)commandLine.c_str(),
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
			LOG(ERROR) << L"Unable to start a process.";
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

	void OS::StartDirectoryWatcher(const std::wstring& dir,
						     	   const std::function<void(const std::wstring&)>& callback,
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

	void OS::StopDirectoryWatcher(const std::wstring& dir)
	{
		const auto& it = local::_sDirectoryWatcherThreads.find(dir);
		// Only have one thread waiting on directory modification
		if (it != local::_sDirectoryWatcherThreads.end())
		{
			it->second.second = false;
			it->second.first.detach();
		}
	}

	std::optional<fs::path> OS::FindFile(const std::wstring& filename,
										 const std::wstring& directory,
										 bool recursive)
	{
		
	}

	std::shared_ptr<std::vector<fs::path>> OS::FindFiles(const std::wstring& filename,
													     const std::wstring& directory,
													     bool recursive)
	{
		static auto _QueryDirectoryFunc = [](const std::wstring& dir, 
											 const std::wstring& name,
											 std::shared_ptr<std::vector<fs::path>> paths,
											 bool recursive)->int {
			std::wstring filter = SZ<wchar_t>::JoinPath(dir, name);
			WIN32_FIND_DATA fd{};
			HANDLE hFind = FindFirstFile(filter.c_str(), &fd);
			if (hFind == INVALID_HANDLE_VALUE) {
				local::CheckLastError("FindFirstFile");
				return 0;
			}

			int count = 0;
			do
			{
				if (wcscmp(fd.cFileName, L".") == 0 ||
					wcscmp(fd.cFileName, L"..") == 0)
					continue;

				paths->emplace_back(SZ<wchar_t>::JoinPath(dir, fd.cFileName));
			} while (FindNextFile(hFind, &fd) != 0);

			return count;
		};

		auto parts = SZ<wchar_t>::Split(SZ<wchar_t>::Canonicalize(filename),
										(wchar_t)PATH_SLASH);
		if (parts.empty())
			return nullptr;

		std::wstring realName = parts[parts.size() - 1];
		auto paths = std::make_shared<std::vector<fs::path>>();
		
		_QueryDirectoryFunc(directory, realName, paths, recursive);
		if (paths->size() == 0)
			return nullptr;
		else if (paths->size() == 1)
		{

		}

		return paths;
	}

	ThreadHandle OS::GetCurrentThread()
	{
		return ::GetCurrentThread();
	}

	void OS::SetThreadAffinity(ThreadHandle thread, uint32_t mask)
	{
		::SetThreadAffinityMask(thread, mask);
		if (DWORD error = GetLastError() != 0)
		{
#if LOG_ENABLED
			LPVOID buffer;
			FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
						  NULL, 
						  error,
						  MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
						  (LPTSTR)&buffer,
						  0,
						  NULL);
			std::wstring errorMessage((LPTSTR)buffer);
			LOG(ERROR) << "SetThreadAffinity() failed: " << errorMessage;
			LocalFree(buffer);
#endif
		}
	}

	void OS::SetThreadPriority(ThreadHandle thread, OS::EThreadPriority priority)
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
			FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
						  NULL,
						  error,
						  MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
						  (LPTSTR)&buffer,
						  0,
						  NULL);
			std::wstring errorMessage((LPTSTR)buffer);
			LOG(ERROR) << "SetThreadPriority() failed: " << errorMessage;
			LocalFree(buffer);
#endif
		}
	}

	time_t OS::GetFileModifiedTime(const std::wstring& filename)
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

	SharedLibraryHandle OS::LoadSharedLibrary(const std::wstring& path)
	{
		return ::LoadLibrary(path.c_str());
	}

	void OS::FreeSharedLibrary(SharedLibraryHandle handle)
	{
		::FreeLibrary(handle);
	}

	void* OS::GetProcAddress(SharedLibraryHandle handle, const std::wstring& func)
	{
		return ::GetProcAddress(handle, func.c_str());
	}
}