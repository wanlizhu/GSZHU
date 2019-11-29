#include "Wanlix/Core3D/Logging/Log.h"
#include <cstdarg>
#include <iostream>

namespace Wanlix
{
    std::mutex Log::mMutex;
    std::weak_ptr<ILogger> Log::mLogger;
    std::ofstream Log::mFileOut;
    bool Log::mLogToFileFlag = false;
    std::vector<LogEntry> Log::mLogBuffer;

    void Log::SetLogger(const std::weak_ptr<ILogger>& logger) {
        mLogger = logger;
    }

    void Log::SetLogFile(StringCRef filename, bool clearFirst)
    {
        FlushBuffer();
        if (mFileOut.is_open()) {
            mFileOut.flush();
            mFileOut.close();
        }

        int mode = std::ofstream::out;
        mode |= (clearFirst ? std::ofstream::trunc : std::ofstream::app);
        mFileOut.open(filename, mode);
    }

    void Log::LogToFile(bool val)
    {
        mLogToFileFlag = val;
    }

    void Log::CloseLogFile()
    {
        if (mFileOut.is_open()) {
            mFileOut.close();
        }
    }

    void Log::Write(const LogType type, const char* fmt, ...)
    {
        char buffer[1024];
        va_list args;
        va_start(args, fmt);
        auto w = vsnprintf(buffer, sizeof(buffer), fmt, args);
        va_end(args);

        if (!buffer) {
            return;
        }

        if (mLogToFileFlag) {
            mLogBuffer.emplace_back(type, buffer); // wait for next logger to flush
            WriteToFile(type, buffer);
            return;
        }

        if (mLogger.expired()) {
            const String prefix = (type == LogInfo) ? "Info:"
                : (type == LogWarning) ? "Warning:"
                : "Error:";
            const auto text = prefix + " " + buffer;
            std::cout << text << std::endl;
            return;
        }
        
        FlushBuffer(); // this will call logger.log()
        WriteToLogger(type, buffer);
    }

    void Log::FlushBuffer()
    {
        if (mFileOut.is_open()) {
            mFileOut.flush();
        }

        if (mLogger.expired() || mLogBuffer.empty()) {
            return;
        }

        for (const auto& entry : mLogBuffer) {
            WriteToLogger(entry.type, entry.message.c_str());
        }
        mLogBuffer.clear();
    }

    void Log::WriteToLogger(LogType type, const char* str)
    {
        std::unique_lock<std::mutex> lock(mMutex);
        if (!str) { 
            return;
        }

        mLogger.lock()->Log(type, str);
    }

    void Log::WriteToFile(LogType type, const char* str)
    {
        std::unique_lock<std::mutex> lock(mMutex);
        if (!str) {
            return;
        }

        const String prefix = (type == LogInfo) ? "Info:" 
            : (type == LogWarning) ? "Warning:" 
            : "Error:";
        const auto text = prefix + " " + str;

        if (!mFileOut.is_open()) {
            Log::SetLogFile("log.txt");
        }

        if (mFileOut.is_open()) {
            mFileOut << text << std::endl;
        }
    }
}