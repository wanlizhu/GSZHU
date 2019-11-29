#pragma once

#include "Wanlix/Core3D/Types.h"
#include "ILogger.h"

#define LOG_INFO(fmt, ...) { Wanlix::Log::Write(Wanlix::LogInfo, String(__FUNCTION__) + ": " + String(fmt), __VA_ARGS__); }
#define LOG_WARNING(fmt, ...) { Wanlix::Log::Write(Wanlix::LogWarning, String(__FUNCTION__) + ": " + String(fmt), __VA_ARGS__); }
#define LOG_ERROR(fmt, ...) { Wanlix::Log::Write(Wanlix::LogError, String(__FUNCTION__) + ": " + String(fmt), __VA_ARGS__); }

namespace Wanlix
{
    struct LogEntry 
    {
        LogType type = LogInfo;
        String message;

        LogEntry(LogType type, StringCRef msg)
            : type(type), message(msg)
        {}
    };

    class Log 
    {
        friend class ILogger;
    public:
        Log() = default;

        // Set a logger to be used (if not set, logging will done in a text file).
        static void SetLogger(const std::weak_ptr<ILogger>& logger);
        static void SetLogFile(StringCRef filename, bool clearFirst = false);
        static void LogToFile(bool val);
        static void CloseLogFile();
        static void Write(const LogType type, const char* fmt, ...);

    private:
        static void FlushBuffer(); // flush to logger
        static void WriteToLogger(LogType type, const char* str);
        static void WriteToFile(LogType type, const char* str);

    private:
        static std::mutex mMutex;
        static std::weak_ptr<ILogger> mLogger;
        static std::ofstream mFileOut;
        static bool mLogToFileFlag;
        static std::vector<LogEntry> mLogBuffer;
    };
}