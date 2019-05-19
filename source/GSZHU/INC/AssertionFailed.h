#pragma once

#include <GSZHU/FormatString.h>
#include <GSZHU/LogOutput.h>
#include "AssertionFailed.h"

namespace GSZHU {
    // defined in DebugOutput.cpp
    extern LOG_MESSAGE_CALLBACK GLOBAL_LogMessageCallback;
    void AssertionFailed(const char* Message, const char* Func, const char* File, int Line);

    template<bool FATAL, typename... ARGS>
    void LogError(const char* Func, const char* File, int Line, const ARGS&... Args) {
        std::string FileName(File);
        auto LastSlash = FileName.find_last_of("/\\");
        if (LastSlash != std::string::npos) {
            FileName.erase(0, LastSlash + 1);
        }

        std::string Message = FormatString(Args...);
        if (GLOBAL_LogMessageCallback != nullptr) {
            GLOBAL_LogMessageCallback(FATAL ? DEBUG_SEVERITY::FATAL_ERROR : DEBUG_SEVERITY::ERROR,
                                      Message.c_str(), Func, FileName.c_str(), Line);
        }
        else {
            std::cerr << "::" << (FATAL ? "Fatal Error" : "Error") << " in " << Func << "() (" << FileName << ", " << Line << "): " << Message << std::endl;
        }

        if (FATAL) {
            throw std::runtime_error(std::move(Message));
        }
    }

    // Log error and fatal_error
#define LOG_ERROR(...)\
do {\
    ::GSZHU::LogError<false>(__FUNCTION__, __FILE__, __LINE__, ##__VA_ARGS__);\
} while(false)

#define LOG_ERROR_IF_NOT(Expr, ...)\
do {\
    if (!(Expr)) {\
        ::GSZHU::LogError<false>(__FUNCTION__, __FILE__, __LINE__, ##__VA_ARGS__);\
    }\
} while(false)

#define LOG_FATAL_ERROR(...)\
do {\
    ::GSZHU::LogError<true>(__FUNCTION__, __FILE__, __LINE__, ##__VA_ARGS__);\
} while(false)

#define LOG_FATAL_ERROR_IF_NOT(Expr, ...)\
do {\
    if (!(Expr)) {\
        ::GSZHU::LogError<true>(__FUNCTION__, __FILE__, __LINE__, ##__VA_ARGS__);\
    }\
} while(false)


    // Log assertion failed in debug mode
#ifdef _DEBUG

#define UNEXPECTED(Message, ...)\
do {\
    auto Str = ::GSZHU::FormatString(Message, ##__VA_ARGS__); \
    AssertionFailed(Str.c_str(), __FUNCTION__, __FILE__, __LINE__);\
} while(false)

#define UNSUPPORTED UNEXPECTED

#define ASSERT(Expr)\
do{\
    if (!(Expr)) {\
        UNEXPECTED("Assertion \"", #Expr " \"failed ");\
    }\
} while(false)

#define CHECK(Expr, Message, ...)\
do {\
    if (!(Expr)) {\
        UNEXPECTED(Message, ##__VA_ARGS__);\
    }\
} while(faslse)

#define CHECK_DYNAMIC_TYPE(DstType, SrcPtr) CHECK(SrcPtr == nullptr || dynamic_cast<DstType*>(SrcPtr) != nullptr, \
                            "Dynamic type cast failed. Source typeid: \'", typeid(*SrcPtr).name(), \
                            "\' Destinat typeid: \'", typeid(DstType).name(), "\'")

#else

#define UNEXPECTED(...) do{}while(false)
#define UNSUPPORTED(...) do{}while(false)
#define ASSERT(...) do{}while(false)
#define CHECK(...) do{}while(false)
#define CHECK_DYNAMIC_TYPE(...) do{}while(false)

#endif

}
