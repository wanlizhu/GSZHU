#include "Wanlix/Logging/ConsoleLogger.h"
#include <iostream>
#include <unordered_map>

#ifdef _WIN32
#include <Windows.h>
#endif

namespace Wanlix
{
    typedef void(ConsoleLogger::* ColorPrintFunc)(const char*) const;
    static std::unordered_map<LogType, ColorPrintFunc> _colorPrintFuncs;

    ConsoleLogger::ConsoleLogger() 
    {
        if (!_colorPrintFuncs.empty()) {
            return;
        }

        _colorPrintFuncs[LogType::LogInfo] = &ConsoleLogger::LogCyan;
        _colorPrintFuncs[LogType::LogWarning] = &ConsoleLogger::LogYellow;
        _colorPrintFuncs[LogType::LogError] = &ConsoleLogger::LogRed;
    }

    void ConsoleLogger::Log(LogType type, const std::string& msg) const
    {
        const std::string prefix = (type == LogInfo) ? "Info:"
            : (type == LogWarning) ? "Warning:"
            : "Error:";
        const auto text = prefix + " " + msg;

        if (_colorPrintFuncs.find(type) == _colorPrintFuncs.end()) {
            LogNoColor(text.c_str());
            return;
        }

        std::invoke(_colorPrintFuncs[type], this, text.c_str());
    }

    void ConsoleLogger::LogNoColor(const char* msg) const
    {
        std::cout << msg << std::endl;
    }

#ifdef _WIN32
    void ConsoleLogger::LogCyan(const char* msg) const
    {
        HANDLE console = GetStdHandle(STD_OUTPUT_HANDLE);
        WORD color = FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_INTENSITY;
        SetConsoleTextAttribute(console, color);

        std::cout << msg << std::endl;

        color = FOREGROUND_RED | FOREGROUND_RED | FOREGROUND_BLUE;
        SetConsoleTextAttribute(console, color);
    }

    void ConsoleLogger::LogYellow(const char* msg) const
    {
        HANDLE console = GetStdHandle(STD_OUTPUT_HANDLE);
        WORD color = FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_INTENSITY;
        SetConsoleTextAttribute(console, color);

        std::cout << msg << std::endl;

        color = FOREGROUND_RED | FOREGROUND_RED | FOREGROUND_BLUE;
        SetConsoleTextAttribute(console, color);
    }

    void ConsoleLogger::LogRed(const char* msg) const
    {
        HANDLE console = GetStdHandle(STD_OUTPUT_HANDLE);
        WORD color = FOREGROUND_RED | FOREGROUND_INTENSITY;
        SetConsoleTextAttribute(console, color);

        std::cout << msg << std::endl;

        color = FOREGROUND_RED | FOREGROUND_RED | FOREGROUND_BLUE;
        SetConsoleTextAttribute(console, color);
    }
#else

#define KREST     "\x1B[0m"
#define KRED      "\x1B[31m"
#define KGREEN    "\x1B[32m"
#define KYELLOW   "\x1B[33m"
#define KBLUE     "\x1B[34m"
#define KMAGENTA  "\x1B[35m"
#define KCYAN     "\x1B[36m"
#define KWHITE    "\x1B[37m"

    void ConsoleLogger::LogCyan(const char* msg) const
    {
        std::cout << KCYAN;
        std::cout << msg << std::endl;
        std::cout << KRESET;
    }

    void ConsoleLogger::LogYellow(const char* msg) const
    {
        std::cout << KYELLOW;
        std::cout << msg << std::endl;
        std::cout << KRESET;
    }

    void ConsoleLogger::LogRed(const char* msg) const
    {
        std::cout << KRED;
        std::cout << msg << std::endl;
        std::cout << KRESET;
    }

#endif
}