#pragma once

#include <Engine/Config.h>
#include <Engine/Core/Common/WindowsFiles.h>
#include <string>
#include <fstream>
#include <sstream>
#include <filesystem>
#include <mutex>

namespace ZHU
{
    class ZHU_API Log
    {
    public:
        enum class ESeverity {
            None,
            Info,
            Debug,
            Warn,
            Error,
            Fatal
        };

        template<typename... ARGS>
        static void Append(ESeverity severity, const std::string& format, ARGS&& ... args) {
            if (severity == ESeverity::Fatal) {
                auto message = StringFormat(format, Log::StringClean(std::forward<ARGS>(args))...);
                AsyncWrite(ESeverity::Fatal, message);
                if (smStream.is_open()) 
                    smStream.close();
                //::MessageBoxA(nullptr, message.c_str(), "Fatal Error", 0);
#if defined(_DEBUG) && defined(_WIN32)
                __debugbreak();
#endif
            }
            else {  

                AsyncWrite(severity, StringFormat(format, Log::StringClean(std::forward<ARGS>(args))...));
            }
        }

    private:
        static void AsyncWrite(ESeverity severity, const std::string& message);
        static void SetColor(HANDLE console, ESeverity severity);
        static WORD GetConsoleAttribs(HANDLE console);
        
        template<typename T>
        static auto StringClean(const T& t) {
            if constexpr (std::is_same_v<std::string, T>) {
                return t.c_str();
            }
            else if constexpr (std::is_same_v<std::filesystem::path, T>) {
                return t.c_str();
            }
            else {
                return t;
            }
        }
        template<typename... ARGS>
        static std::string StringFormat(const std::string& format, ARGS&& ... args) {
            auto size{ std::snprintf(nullptr, 0, format.c_str(), args...) };
            std::string output(size, '\0');
            std::sprintf(&output[0], format.c_str(), std::forward<ARGS>(args)...);
            return output;
        }

    private:
        static std::mutex smMutex;
        static std::ofstream smStream;
    };
}

#ifdef _DEBUG
#define LOG_INFO(format, ...) ZHU::Log::Append(ZHU::Log::ESeverity::Info, "[%s: %i] "##format, __FUNCTION__, __LINE__, __VA_ARGS__)
#define LOG_DEBUG(format, ...) ZHU::Log::Append(ZHU::Log::ESeverity::Debug, "[%s: %i] "##format, __FUNCTION__, __LINE__, __VA_ARGS__)
#define LOG_WARNING(format, ...) ZHU::Log::Append(ZHU::Log::ESeverity::Warn, "[%s: %i] "##format, __FUNCTION__, __LINE__, __VA_ARGS__)
#else
#define LOG_INFO(format, ...) 
#define LOG_DEBUG(format, ...)
#define LOG_WARNING(format, ...)
#endif
#define LOG_ERROR(format, ...) ZHU::Log::Append(ZHU::Log::ESeverity::Error, "[%s: %i] "##format, __FUNCTION__, __LINE__, __VA_ARGS__)
#define LOG_FATAL(format, ...) ZHU::Log::Append(ZHU::Log::ESeverity::Fatal, "[%s: %i] "##format, __FUNCTION__, __LINE__, __VA_ARGS__)

#define LOG_INFO_IF(expr, format, ...) if (!!(expr)) LOG_INFO(format, __VA_ARGS__)
#define LOG_DEBUG_IF(expr, format, ...) if (!!(expr)) LOG_DEBUG(format, __VA_ARGS__)
#define LOG_WARNING_IF(expr, format, ...) if (!!(expr)) LOG_WARNING(format, __VA_ARGS__)
#define LOG_ERROR_IF(expr, format, ...) if (!!(expr)) LOG_ERROR(format, __VA_ARGS__)
#define LOG_FATAL_IF(expr, format, ...) if (!!(expr)) LOG_FATAL(format, __VA_ARGS__)

#define SHOULDNT_HAVE_BEEN_HERE() LOG_FATAL("!!!Shouldn't Have Been Here!!!")
#define ASSERT(expr) LOG_FATAL_IF(expr, "Assertion Failed: '%s'", #expr)