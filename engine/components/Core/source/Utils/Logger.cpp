#include "Utils/Logger.h"
#include <cstdarg>
#include <iostream>
#include "PAL/System.h"

#if defined _WIN32
#include <Windows.h>
#endif

#ifdef ENABLE_LOG

namespace Wanlix
{
    std::ofstream Logger::mLogFile;
    bool Logger::mLogToFileEnabled = true;
    bool Logger::mDialogOnErrorEnabled = true;
    TaskThread Logger::mLogThread;

    void Logger::SetLogFile(const std::string& filename, bool clearFirst)
    {
        if (mLogFile.is_open()) {
            mLogFile.flush();
            mLogFile.close();
        }

        int mode = std::ofstream::out;
        mode |= (clearFirst ? std::ofstream::trunc : std::ofstream::app);
        mLogFile.open(filename, mode);
    }

    void Logger::LogToFile(bool val)
    {
        mLogToFileEnabled = val;
    }

    void Logger::ShowDialogOnError(bool val)
    {
        mDialogOnErrorEnabled = val;
    }

    bool Logger::IsEnabled()
    {
        return true;
    }

    bool Logger::Initialize()
    {
        SetLogFile(JoinPath(GetCWD(), GetExecutableName() + ".log"), true);
        return true;
    }

    void Logger::Shutdown()
    {
        if (mLogFile.is_open()) {
            mLogFile.close();
        }
    }

    void Logger::Log(ELevel level, const char* fmt, ...)
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

    void Logger::WriteToConsole(ELevel level, const char* str)
    {
        mLogThread.AddTask([&]() {
            SetConsoleColor(level);
            printf(str);
            printf("\n");
            SetConsoleColor(ELevel::Disabled);
            fflush(stdout);
        });
    }

    void Logger::WriteToFile(ELevel level, const char* str)
    {
        mLogThread.AddTask([&]() {
            if (mLogFile.is_open() && mLogToFileEnabled) {
                mLogFile << str << "\n";
                mLogFile.flush();
            }
        });
    }

#ifdef _WIN32
    void Logger::SetConsoleColor(ELevel level)
    {
        HANDLE console = GetStdHandle(STD_OUTPUT_HANDLE);

        if (level == ELevel::Disabled || level == ELevel::Info) {
            WORD color = FOREGROUND_RED | FOREGROUND_RED | FOREGROUND_BLUE;
            SetConsoleTextAttribute(console, color);
        }

        if (level == ELevel::Warning) {
            WORD color = FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_INTENSITY;
            SetConsoleTextAttribute(console, color);
        }

        if (level == ELevel::Debug) {
            WORD color = FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_INTENSITY;
            SetConsoleTextAttribute(console, color);
        }

        if (level == ELevel::Error || level == ELevel::Fatal) {
            WORD color = FOREGROUND_RED | FOREGROUND_INTENSITY;
            SetConsoleTextAttribute(console, color);
        }
    }
#else
    void Logger::SetConsoleColor(ELevel level)
    {
    }
#endif
}

#endif