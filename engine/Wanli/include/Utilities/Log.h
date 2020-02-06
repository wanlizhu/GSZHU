#pragma once

#include <string>
#include <string_view>
#include <filesystem>
#include <fstream>
#include <sstream>
#include <mutex>
#include <thread>
#include <queue>
#include <iostream>
#include "Config.h"
#include "BasicTypes.h"
#ifdef ENABLE_ASYNC_LOG
#include "ThreadPool.h"
#endif

namespace Wanli
{
	struct LogStyle
	{
		static constexpr const char* Default = "\033[0m";
		static constexpr const char* Bold = "\033[1m";
		static constexpr const char* Dim = "\033[2m";
		static constexpr const char* Underlined = "\033[4m";
		static constexpr const char* Blink = "\033[5m";
		static constexpr const char* Reverse = "\033[7m";
		static constexpr const char* Hidden = "\033[8m";
	};

	struct LogColor
	{
		static constexpr const char* Default = "\033[39m";
		static constexpr const char* Black = "\033[30m";
		static constexpr const char* Red = "\033[31m";
		static constexpr const char* Green = "\033[32m";
		static constexpr const char* Yellow = "\033[33m";
		static constexpr const char* Blue = "\033[34m";
		static constexpr const char* Magenta = "\033[35m";
		static constexpr const char* Cyan = "\033[36m";
		static constexpr const char* LightGrey = "\033[37m";
		static constexpr const char* DarkGrey = "\033[90m";
		static constexpr const char* LightRed = "\033[91m";
		static constexpr const char* LightGreen = "\033[92m";
		static constexpr const char* LightYellow = "\033[93m";
		static constexpr const char* LightBlue = "\033[94m";
		static constexpr const char* LightMagenta = "\033[95m";
		static constexpr const char* LightCyan = "\033[96m";
		static constexpr const char* White = "\033[97m";
	};

    class DLLDECL Log
    {
    public:
		static constexpr auto kTimestampFormat = "%H:%M:%S";
		static void Initialize();
		static void OpenLog(const Path& path);
		static void CloseLog();

		template<typename... _Args_>
		static inline void WriteAsync(const std::string_view& style,
									  const std::string_view& color,
									  const char* file,
									  int line,
									  _Args_... args)
		{
#ifdef ENABLE_ASYNC_LOG
			ThreadPool::AddTask(std::bind(&Log::Write, this));
#else
			Write(style, color, file, line, std::forward<_Args_>(args)...);
#endif
		}

		template<typename... _Args_>
		static inline void Write(const std::string_view& style,
								 const std::string_view& color,
								 const char* file,
								 int line,
								 _Args_... args)
		{
			std::unique_lock<std::mutex> lock(mWriteMutex);
			std::string prefix = std::string(file) + "(" + std::to_string(line) + "): ";
			if (prefix == mLastPrefix)
			{
				prefix.assign(prefix.size(), ' ');
			}

			SetStyleAndColor(style, color);
			std::cout << prefix;
			((std::cout << std::forward<_Args_>(args)), ...);
			SetStyleAndColor();

			if (mFileStream.is_open())
			{
				mFileStream << prefix;
				((mFileStream << std::forward<_Args_>(args)), ...);
			}
		}

	private:
		static void SetStyleAndColor(const std::string_view& style = LogStyle::Default,
									 const std::string_view& color = LogColor::Default);

	private:
		static std::ofstream mFileStream;
		static std::mutex mWriteMutex;
		static std::string mLastPrefix;
	};

#define LOG_ERROR(...)   Log::WriteAsync(LogStyle::Bold, LogColor::Red, __FILE__, __LINE__, __VA_ARGS__)
#define LOG_WARNING(...) Log::WriteAsync(LogStyle::Default, LogColor::Yellow, __FILE__, __LINE__, __VA_ARGS__)
#define	LOG_DEBUG(...)   Log::WriteAsync(LogStyle::Default, LogColor::Cyan, __FILE__, __LINE__, __VA_ARGS__)
#define LOG_INFO(...)    Log::WriteAsync(LogStyle::Default, LogColor::Default, __FILE__, __LINE__, __VA_ARGS__)
}