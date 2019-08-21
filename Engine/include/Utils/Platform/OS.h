#pragma once

#include <string>
#include <vector>
#include <thread>
#include <functional>
#include "Framework.h"

namespace GS
{
	class Window;

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

		static void SetWindowIcon(const std::string& iconFile, WindowHandle handle);
		static int GetDisplayDPI();
		static float GetDisplayScaleFactor();

		static EMessageBoxButton ShowMessageBox(const std::string& msg, 
												EMessageBoxType type = EMessageBoxType::Ok);
		static int FindFiles(std::string filter, std::vector<std::string>* files);
		static std::string FindFile(const std::string& filename, const std::string& directory);
		static std::string FindDataFile(const std::string& filename);
		static bool OpenFileDialog(const std::vector<FileDialogFilter>& filters, std::string* filename);
		static bool SaveFileDialog(const std::vector<FileDialogFilter>& filters, std::string* filename);
		
		static bool FileExists(const std::string& filename);
		static bool DirectoryExists(const std::string& filename);
		static void StartFileWatcher(const std::string& filename, const std::function<void()>& callback = {});
		static void StopFileWatcher(const std::string& filename);

		static bool MakeFile(const std::string& filename);
		static bool MakeDirectory(const std::string& filename);

		static size_t ExecuteProcess(const std::string& appname, const std::string& args);
		static bool IsProcessActive(size_t id);
		static void TerminateProcess(size_t id);

		static std::string GetPWD();
		static std::string GetExecutableName();
		static std::string GetTMPFile();
		static bool ReadEnvironmentVariable(const std::string& name, std::string* value);
		
		static const std::vector<std::string>& GetDataDirectory();
		static void AddDataDirectory(const std::string& directory);
		static void RemoveDataDirectory(const std::string& directory);

		static bool FindAvailableFileName(const std::string& basename, 
										  const std::string& ext,
										  const std::string& directory,
										  std::string* result);
		static bool IsDebuggerAttached();
		static void DebugBreak();
		static void DebugOutput(const std::string& msg);
		static std::string Canonicalize(const std::string& filename);
		static std::string GetExtension(const std::string& filename);
		static std::string GetBaseName(const std::string& filename);
		static std::string GetDirectory(const std::string& filename);

		static std::thread::native_handle_type GetCurrentThread();
		static void SetThreadAffinity(std::thread::native_handle_type thread, uint32_t mask);
		static time_t GetFileModifiedTime(const std::string& filename);
		static void SetThreadPriority(std::thread::native_handle_type thread, EThreadPriority priority);
		
		static uint64_t GetTotalVirtualMemory();
		static uint64_t GetUsedVirtualMemory();
		static uint64_t GetProcessUsedVirtualMemory();

		static size_t LoadFile(const std::string& filename, std::vector<unsigned char>* data);
		static size_t LoadFile(const std::string& filename, std::string* data);
		
		static SharedLibraryHandle LoadSharedLibrary(const std::string& path);
		static void FreeSharedLibrary(SharedLibraryHandle handle);
		static void* GetProcAddress(SharedLibraryHandle handle, const std::string& func);
	};
}