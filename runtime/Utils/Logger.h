#pragma once

#include <string>
#include <fstream>
#include "TaskThread.h"

#define LOG_INFO(fmt, ...) { Wanlix::Logger::Log(Wanlix::Logger::ELevel::Info, fmt, __VA_ARGS__); }
#define LOG_WARNING(fmt, ...) { Wanlix::Logger::Log(Wanlix::Logger::ELevel::Warning, fmt, __VA_ARGS__); }
#define LOG_DEBUG(fmt, ...) { Wanlix::Logger::Log(Wanlix::Logger::ELevel::Debug, fmt, __VA_ARGS__); }
#define LOG_ERROR(fmt, ...) { Wanlix::Logger::Log(Wanlix::Logger::ELevel::Error, fmt, __VA_ARGS__); }
#define LOG_FATAL(fmt, ...) { Wanlix::Logger::Log(Wanlix::Logger::ELevel::Fatal, fmt, __VA_ARGS__); }

namespace Wanlix
{
    class Logger final
    {
    public:
        enum class ELevel
        {
            Disabled = 0,
            Info,
            Warning,
            Debug,
            Error,
            Fatal,
        };

        static bool Initialize();
        static void Shutdown();
        static void SetLogFile(const std::wstring& filename, bool clearFirst = false);
        static void LogToFile(bool val);
        static void ShowDialogOnError(bool val);
        static bool IsEnabled();
        static void Log(ELevel level, const char* fmt, ...);

    private:
        static void WriteToConsole(ELevel level, const char* str);
        static void WriteToFile(ELevel level, const char* str);
        static void SetConsoleColor(ELevel level);

    private:
        static std::ofstream mLogFile;
        static bool mLogToFileEnabled;
        static bool mDialogOnErrorEnabled;
        static TaskThread mLogThread;
    };
}