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
#include "Core/BasicTypes.h"
#ifdef ENABLE_ASYNC_LOG
#include "Utilities/ThreadPool.h"
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

		template<typename... Args>
		static inline void WriteAsync(const std::string_view& style,
									  const std::string_view& color,
			                          Args... args)
		{
#if 0//defined ENABLE_ASYNC_LOG
			ThreadPool::Enqueue(
				&Log::Write,
				style,
				color,
				std::forward<Args>(args)...
			);
#else
			Write(style, color, std::forward<Args>(args)...);
#endif
		}

		template<typename... Args>
		static inline void Write(const std::string_view& style,
								 const std::string_view& color,
								 Args... args)
		{
			SetStyleAndColor(style, color);
			((std::cout << std::forward<Args>(args)), ...);
			SetStyleAndColor();

			if (mFileStream.is_open())
			{
				//mFileStream << prefix;
				((mFileStream << std::forward<Args>(args)), ...);
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

#define LOG_ERROR(...)   Log::WriteAsync(LogStyle::Bold, LogColor::Red, __VA_ARGS__)
#define LOG_WARNING(...) Log::WriteAsync(LogStyle::Default, LogColor::Yellow, __VA_ARGS__)
#define	LOG_DEBUG(...)   Log::WriteAsync(LogStyle::Default, LogColor::Cyan, __VA_ARGS__)
#define LOG_INFO(...)    Log::WriteAsync(LogStyle::Default, LogColor::Default, __VA_ARGS__)
}