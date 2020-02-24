#include "Utilities/Log.h"

namespace Wanli
{
	std::ofstream Log::mFileStream;
	std::mutex Log::mWriteMutex;
	std::string Log::mLastPrefix = "";

	void Log::Initialize()
	{
		std::chrono::system_clock::time_point now = std::chrono::system_clock::now();
		std::time_t now_c = std::chrono::system_clock::to_time_t(now);
		std::tm now_tm = *std::localtime(&now_c);
		char buffer[100];
		strftime(buffer, 100, "%Y-%m-%d %H:%M:%S", &now_tm);
		OpenLog(std::string(buffer) + ".log");
	}

	void Log::OpenLog(const Path& path)
	{
		if(auto parentPath = path.parent_path(); !parentPath.empty())
		{
			std::filesystem::create_directories(parentPath);
		}
		mFileStream.open(path);
	}

	void Log::CloseLog()
	{
		mFileStream.close();
	}

	void Log::SetStyleAndColor(const char* style,
							   const char* color)
	{
		std::cout << style << color;
	}
}