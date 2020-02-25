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
#include <future>
#include "Utils/String.h"

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

    class Log
    {
    public:
		static constexpr auto kTimestampFormat = "%H:%M:%S";
		static void Initialize();
		static void OpenLog(const Path& path);
		static void CloseLog();

		template<typename... Args>
		static inline void WriteAsync(
			const char* style,
			const char* color,
			Args... args)
		{
			(void)std::async(
				std::launch::async,
				&Log::Write<Args...>,
				style,
				color,
				std::forward<Args>(args)...);
		}

		template<typename... Args>
		static inline void Write(
			const char* style,
			const char* color,
			Args... args)
		{
			String finalMsg = FormatString(std::forward<Args>(args)...);

			SetStyleAndColor(style, color);
			std::cout << finalMsg;
			SetStyleAndColor();

			if (mFileStream.is_open())
			{
				mFileStream << finalMsg;
			}
		}

	private:
		static void SetStyleAndColor(const char* style = LogStyle::Default, const char* color = LogColor::Default);

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