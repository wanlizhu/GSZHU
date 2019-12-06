#include "Wanlix/Log.h"
#include <cstdarg>
#include <iostream>

#if defined _WIN32
#include <Windows.h>
#endif

namespace Wanlix
{
    std::ofstream Log::mLogFile;
    bool Log::mLogToFileEnabled = true;
    bool Log::mDialogOnErrorEnabled = true;
    TaskThread Log::mLogThread;

    void Log::SetLogFile(const std::string& filename, bool clearFirst)
    {
        if (mLogFile.is_open()) {
            mLogFile.flush();
            mLogFile.close();
        }

        int mode = std::ofstream::out;
        mode |= (clearFirst ? std::ofstream::trunc : std::ofstream::app);
        mLogFile.open(filename, mode);
    }

    void Log::LogToFile(bool val)
    {
        mLogToFileEnabled = val;
    }

    void Log::ShowDialogOnError(bool val)
    {
        mDialogOnErrorEnabled = val;
    }

    void Log::CloseLogFile()
    {
        if (mLogFile.is_open()) {
            mLogFile.close();
        }
    }

    void Log::Write(LogLevel level, const char* fmt, ...)
    {
        static char buffer[1024];
        va_list args;
        va_start(args, fmt);
        auto w = vsnprintf(buffer, sizeof(buffer), fmt, args);
        va_end(args);

        if (!buffer) {
            return;
        }

        WriteToConsole(level, buffer);

        if (mLogToFileEnabled) {
            WriteToFile(level, buffer);
        }
    }

    void Log::WriteToConsole(LogLevel level, const char* str)
    {
        mLogThread.AddTask([&]() {
            SetConsoleColor(level);
            printf(str);
            printf("\n");
            SetConsoleColor(LogNone);
            fflush(stdout);
        });
    }

    void Log::WriteToFile(LogLevel level, const char* str)
    {
        mLogThread.AddTask([&]() {
            if (mLogFile.is_open() && mLogToFileEnabled) {
                mLogFile << str << "\n";
                mLogFile.flush();
            }
        });
    }

#ifdef _WIN32
    void Log::SetConsoleColor(LogLevel level)
    {
        HANDLE console = GetStdHandle(STD_OUTPUT_HANDLE);

        if (level == LogNone || level == LogInfo) {
            WORD color = FOREGROUND_RED | FOREGROUND_RED | FOREGROUND_BLUE;
            SetConsoleTextAttribute(console, color);
        }

        if (level == LogWarning) {
            WORD color = FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_INTENSITY;
            SetConsoleTextAttribute(console, color);
        }

        if (level == LogDebug) {
            WORD color = FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_INTENSITY;
            SetConsoleTextAttribute(console, color);
        }

        if (level == LogError) {
            WORD color = FOREGROUND_RED | FOREGROUND_INTENSITY;
            SetConsoleTextAttribute(console, color);
        }
    }
#else
    void Log::SetConsoleColor(LogLevel level)
    {
    }
#endif
}