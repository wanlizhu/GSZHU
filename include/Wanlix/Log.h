#pragma once

#include <string>
#include <fstream>
#include "TaskThread.h"

#define LOG_INFO(fmt, ...) { Wanlix::Log::Write(Wanlix::LogInfo, std::string(__FUNCTION__) + ": " + std::string(fmt), __VA_ARGS__); }
#define LOG_WARNING(fmt, ...) { Wanlix::Log::Write(Wanlix::LogWarning, std::string(__FUNCTION__) + ": " + std::string(fmt), __VA_ARGS__); }
#define LOG_ERROR(fmt, ...) { Wanlix::Log::Write(Wanlix::LogError, std::string(__FUNCTION__) + ": " + std::string(fmt), __VA_ARGS__); }

namespace Wanlix
{
    enum LogLevel
    {
        LogNone,
        LogInfo,
        LogWarning,
        LogDebug,
        LogError,
    };

    class Log 
    {
    public:
        static void SetLogFile(const std::string& filename, bool clearFirst = false);
        static void LogToFile(bool val);
        static void ShowDialogOnError(bool val);
        static void CloseLogFile();
        static void Write(LogLevel level, const char* fmt, ...);

    private:
        static void WriteToConsole(LogLevel level, const char* str);
        static void WriteToFile(LogLevel level, const char* str);
        static void SetConsoleColor(LogLevel level);

    private:
        static std::ofstream mLogFile;
        static bool mLogToFileEnabled;
        static bool mDialogOnErrorEnabled;
        static TaskThread mLogThread;
    };
}