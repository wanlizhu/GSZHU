#include "ENGINE_PCH.h"
#include <iostream>
#include <thread>
#include <future>
#include <Engine/Core/Common/Log.h>
#include <Engine/Core/Common/EnumClass.h>
#include <Engine/Core/Common/BasicTypes.h>
#include "./OS.h"

#define FOREGROUND_WHITE (FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE)
#define FOREGROUND_CYAN (FOREGROUND_GREEN | FOREGROUND_BLUE)
#define FOREGROUND_MAGENTA (FOREGROUND_RED | FOREGROUND_BLUE)
#define FOREGROUND_YELLOW (FOREGROUND_RED | FOREGROUND_GREEN)

namespace ZHU
{
    std::mutex Log::smMutex;
    std::ofstream Log::smStream;

    BEGIN_ENUM_FIELDS(Log::ESeverity)
        ADD_ENUM_FIELD2(Log::ESeverity::None, "None"),
        ADD_ENUM_FIELD2(Log::ESeverity::Info, "Info"),
        ADD_ENUM_FIELD2(Log::ESeverity::Debug, "Debug"),
        ADD_ENUM_FIELD2(Log::ESeverity::Warn, "Warn"),
        ADD_ENUM_FIELD2(Log::ESeverity::Error, "Error"),
        ADD_ENUM_FIELD2(Log::ESeverity::Fatal, "Fatal Error")
    END_ENUM_FIELDS

    void Log::AsyncWrite(ESeverity severity, const std::string& message) 
    {
        auto future = std::async(std::launch::async, [&] () {
            std::lock_guard<std::mutex> lock(smMutex);

            HANDLE console = ::GetStdHandle(STD_OUTPUT_HANDLE);
            WORD prevAttribs = GetConsoleAttribs(console);
            SetColor(console, severity);
            std::cout << ENUM_TO_NAME(severity) << " " << message << std::endl;
            ::SetConsoleTextAttribute(console, prevAttribs);

            if (!smStream.is_open()) {
                smStream.open(GetCurrentFileName() + ".log");
                assert(smStream.is_open());
            }

            smStream << ENUM_TO_NAME(severity) << " " << message << std::endl;
            smStream.flush();
        });
    }

    WORD Log::GetConsoleAttribs(HANDLE console)
    {
        assert(console != INVALID_HANDLE_VALUE);
        return FOREGROUND_WHITE;
    }

    void Log::SetColor(HANDLE console, ESeverity severity) 
    {
        WORD attrib = 0;
        switch (severity) {
        case ESeverity::None:
        case ESeverity::Info:
            attrib = FOREGROUND_WHITE | FOREGROUND_INTENSITY;
            break;
        case ESeverity::Debug:
            attrib = FOREGROUND_CYAN | FOREGROUND_INTENSITY;
            break;
        case ESeverity::Warn:
            attrib = FOREGROUND_MAGENTA | FOREGROUND_INTENSITY;
            break;
        case ESeverity::Error:
        case ESeverity::Fatal:
            attrib = FOREGROUND_WHITE | BACKGROUND_RED | BACKGROUND_INTENSITY;
            break;
        default:
            assert(false);
        }

        ::SetConsoleTextAttribute(console, attrib);
    }
}