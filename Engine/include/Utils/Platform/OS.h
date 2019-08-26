#pragma once

#include <string>
#include <vector>
#include <thread>
#include <functional>
#include <optional>
#include "Framework.h"

namespace GS
{
	class GS_API OS
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
			std::string desc;
			std::string ext;
		};


		static  bool SetWindowIcon(const std::string& iconFile, WindowHandle handle);
		static   int GetDisplayDPI();
		static float GetDisplayScaleFactor();

		static EMessageBoxButton ShowMessageBox(const std::string& msg,
				   		                        EMessageBoxType type = EMessageBoxType::Ok);
		static std::vector<std::string> FindFiles(const std::string& filename,
					                              const std::string& directory,
					                              bool recursive = true,
					                              bool findAll = true);
		static std::optional<std::string> FindFile(const std::string& filename,
					                               const std::string& directory,
					                               bool recursive = true);
		static std::optional<std::string> FindDataFile(const std::string& filename);
		static std::optional<std::string> OpenFileDialog(const std::vector<FileDialogFilter>& filters);
		static std::optional<std::string> SaveFileDialog(const std::vector<FileDialogFilter>& filters);
		
		static bool IsAbsolutePath(const std::string& filename);
		static bool FileExists(const std::string& filename);
		static bool DirectoryExists(const std::string& filename);
		static void StartDirectoryWatcher(const std::string& dir, 
										  const std::function<void(const std::string&)>& callback,
										  bool watchSubtree = true);
		static void StopDirectoryWatcher(const std::string& dir);

		static bool MakeFile(const std::string& filename);
		static bool MakeDirectory(const std::string& filename);

		static size_t ExecuteProcess(const std::string& appname, const std::string& args);
		static bool IsProcessActive(size_t id);
		static void TerminateProcess(size_t id);

		static std::string GetPWD();
		static std::string GetExecutableDirectory();
		static std::string GetExecutableName();
		static std::string GetTMPDirectory();
		static char GetPreferredSeparator();
		static std::optional<std::string> EnvironmentVariable(const std::string& name);
		
		static const std::vector<std::string>& GetDataDirectories();
		static void AddDataDirectory(const std::string& directory);
		static void RemoveDataDirectory(const std::string& directory);

		static std::string FindAvailableFileName(const std::string& basename, 
										         const std::string& ext,
										         const std::string& directory);
		static bool IsDebuggerPresent();
		static void DebugBreak();
		static void DebugOutput(const std::string& msg);
		static std::string Canonicalize(const std::string& filename);
		static std::string GetExtension(const std::string& filename);
		static std::string GetFileName(const std::string& filename);
		static std::string GetStemName(const std::string& filename);
		static std::string GetDirectory(const std::string& filename);

		static std::thread::native_handle_type GetCurrentThread();
		static void SetThreadAffinity(std::thread::native_handle_type thread, uint32_t mask);
		static time_t GetFileModifiedTime(const std::string& filename);
		static void SetThreadPriority(std::thread::native_handle_type thread, EThreadPriority priority);
		
		static uint64_t GetTotalVirtualMemory();
		static uint64_t GetUsedVirtualMemory();
		static uint64_t GetProcessUsedVirtualMemory();

		static std::shared_ptr<std::string> LoadFile(const std::string& filename);
		static size_t SaveFile(const std::string& filename, const void* data, size_t size, bool append = false);
		
		static SharedLibraryHandle LoadSharedLibrary(const std::string& path);
		static void FreeSharedLibrary(SharedLibraryHandle handle);
		static void* GetProcAddress(SharedLibraryHandle handle, const std::string& func);
	};
}