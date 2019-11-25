#pragma once

#include <string>
#include <memory>
#include <mutex>
#include <fstream>
#include <vector>
#include "Wanlix/Core/EngineConfig.h"
#include "ILogger.h"

#define LOG_INFO(fmt, ...) { Wanlix::Log::Write(Wanlix::LogInfo, std::string(__FUNCTION__) + ": " + std::string(fmt), __VA_ARGS__); }
#define LOG_WARNING(fmt, ...) { Wanlix::Log::Write(Wanlix::LogWarning, std::string(__FUNCTION__) + ": " + std::string(fmt), __VA_ARGS__); }
#define LOG_ERROR(fmt, ...) { Wanlix::Log::Write(Wanlix::LogError, std::string(__FUNCTION__) + ": " + std::string(fmt), __VA_ARGS__); }

namespace Wanlix
{
    struct LogEntry 
    {
        LogType type = LogInfo;
        std::string message;

        LogEntry(LogType type, const std::string& msg)
            : type(type)
            , message(msg)
        {}
    };

    class Log 
    {
        friend class ILogger;
    public:
        Log() = default;

        // Set a logger to be used (if not set, logging will done in a text file).
        static void SetLogger(const std::weak_ptr<ILogger>& logger);
        static void SetLogFile(
            const std::string& filename,
            bool clearFirst = false
        );
        static void LogToFile(bool val);
        static void CloseLogFile();
        static void Write(
            const LogType type,
            const char* fmt,
            ...
        );

    private:
        static void FlushBuffer(); // flush to logger
        static void WriteToLogger(
            LogType type,
            const char* str
        );
        static void WriteToFile(
            LogType type,
            const char* str
        );

    private:
        static std::mutex mMutex;
        static std::weak_ptr<ILogger> mLogger;
        static std::ofstream mFileOut;
        static bool mLogToFileFlag;
        static std::vector<LogEntry> mLogBuffer;
    };
}