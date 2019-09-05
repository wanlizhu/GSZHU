#pragma once

#include <string>
#include <vector>
#include <thread>
#include <functional>
#include <optional>
#include "Framework.h"

namespace GS
{
	class GS_API OS//todo
	{
	public:
		enum class EMessageBoxType
		{
			Ok,
			OkCancel,
			RetryCancel,
			AbortRetryCancel,
		};

		enum class EMessageBoxButton
		{
			Ok,
			Retry,
			Cancel,
			Abort,
			Ignore,
		};

		enum class EThreadPriority
		{
			BackgroundBegin = -2,   // Indicates I/O-intense thread
			BackgroundEnd = -1,     // Indicates the end of I/O-intense operations in the thread
			Lowest = 0,             // Lowest priority
			Low = 1,
			Normal = 2,
			High = 3,
			Highest = 4,
		};

		struct FileDialogFilter
		{
			std::wstring desc;
			std::wstring ext;
		};

		class GS_API FileIterator
		{
		public:
			//todo
		};


		static  bool SetWindowIcon(const std::wstring& iconFile, WindowHandle handle);
		static   int GetDisplayDPI();
		static float GetDisplayScaleFactor();

		static EMessageBoxButton ShowMessageBox(const std::wstring& msg,
				   		                        EMessageBoxType type = EMessageBoxType::Ok);
		static FileIterator CreateFileIterator(const std::wstring& filename,
											   const std::wstring& directory,
											   bool recursive = true);
		static std::shared_ptr<std::vector<fs::path>> FindFiles(const std::wstring& filename,
																const std::wstring& directory,
																bool recursive = true);
		static std::shared_ptr<std::vector<fs::path>> FindDataFiles(const std::wstring& filename);
		static std::optional<fs::path> OpenFileDialog(const std::vector<FileDialogFilter>& filters);
		static std::optional<fs::path> SaveFileDialog(const std::vector<FileDialogFilter>& filters);
		
		static bool IsAbsolutePath(const std::wstring& filename);
		static bool FileExists(const std::wstring& filename);
		static bool DirectoryExists(const std::wstring& filename);
		static void StartDirectoryWatcher(const std::wstring& dir, 
										  const std::function<void(const std::wstring&)>& callback,
										  bool watchSubtree = true);
		static void StopDirectoryWatcher(const std::wstring& dir);

		static bool MakeFile(const std::wstring& filename);
		static bool MakeDirectory(const std::wstring& filename);

		static size_t ExecuteProcess(const std::wstring& appname, const std::wstring& args);
		static bool IsProcessActive(size_t id);
		static void TerminateProcess(size_t id);

		static std::wstring GetPWD();
		static std::wstring GetExecutableDirectory();
		static std::wstring GetExecutableName();
		static std::wstring GetTMPDirectory();
		static std::optional<std::wstring> EnvironmentVariable(const std::wstring& name);
		
		static const std::vector<std::wstring>& GetDataDirectories();
		static void AddDataDirectory(const std::wstring& directory);
		static void RemoveDataDirectory(const std::wstring& directory);

		static std::wstring FindAvailableFileName(const std::wstring& basename, 
										          const std::wstring& ext,
										          const std::wstring& directory);
		static bool IsDebuggerPresent();
		static void DebugBreak();
		static void DebugOutput(const std::wstring& msg);
		static std::wstring GetExtension(const std::wstring& filename);
		static std::wstring GetFileName(const std::wstring& filename);
		static std::wstring GetStemName(const std::wstring& filename);
		static std::wstring GetDirectory(const std::wstring& filename);

		static ThreadHandle GetCurrentThread();
		static void SetThreadAffinity(ThreadHandle thread, uint32_t mask);
		static time_t GetFileModifiedTime(const std::wstring& filename);
		static void SetThreadPriority(ThreadHandle thread, EThreadPriority priority);
		
		static uint64_t GetTotalVirtualMemory();
		static uint64_t GetUsedVirtualMemory();
		static uint64_t GetProcessUsedVirtualMemory();

		static std::shared_ptr<std::string> LoadFile(const std::wstring& filename);
		static size_t SaveFile(const std::wstring& filename, const void* data, size_t size, bool append = false);
		
		static SharedLibraryHandle LoadSharedLibrary(const std::wstring& path);
		static void FreeSharedLibrary(SharedLibraryHandle handle);
		static void* GetProcAddress(SharedLibraryHandle handle, const std::string& func);
	};
}