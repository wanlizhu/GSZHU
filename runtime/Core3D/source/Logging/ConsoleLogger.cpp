#include "Wanlix/Core3D/Logging/ConsoleLogger.h"
#include <iostream>
#include <unordered_map>
#include <functional>

#ifdef _WIN32
#include <Windows.h>
#endif

namespace Wanlix
{
    static std::unordered_map<LogType, std::function<void(const char*)>> g_colorPrintFuncs;

    ConsoleLogger::ConsoleLogger() noexcept
    {
        if (!g_colorPrintFuncs.empty()) {
            return;
        }

        g_colorPrintFuncs[LogType::LogInfo] = [this](const char* msg) { LogCyan(msg); };
        g_colorPrintFuncs[LogType::LogWarning] = [this](const char* msg) { LogYellow(msg); };
        g_colorPrintFuncs[LogType::LogError] = [this](const char* msg) { LogRed(msg); };
    }

    void ConsoleLogger::Log(LogType type, const std::string& msg) const
    {
        const std::string prefix = (type == LogInfo) ? "Info:"
            : (type == LogWarning) ? "Warning:"
            : "Error:";
        const auto text = prefix + " " + msg;

        if (g_colorPrintFuncs.find(type) == g_colorPrintFuncs.end()) {
            LogNoColor(text.c_str());
            return;
        }

        g_colorPrintFuncs[type](text.c_str());
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