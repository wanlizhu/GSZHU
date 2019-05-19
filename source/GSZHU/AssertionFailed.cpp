#include "INC/AssertionFailed.h"

#include <iostream>
#include <string>
#include <Windows.h>
#include <GSZHU/FormatString.h>
#ifdef GSZHU_OS_WINDOWS
#include <Windows.h>
#include <signal.h>
#endif

namespace GSZHU {
#ifdef GSZHU_OS_WINDOWS
    void AssertionFailed(const char* Message, const char* Func, const char* File, int Line) {
        std::string FileName(File);
        auto LastSlash = FileName.find_last_of("/\\");
        if (LastSlash != std::string::npos) {
            FileName.erase(0, LastSlash + 1);
        }

        auto Str = ::GSZHU::FormatString("Assertion failed in ", Func, "(), (", FileName, ", ", Line, "): ", Message);
        int nCode = MessageBoxA(NULL,
                                Str.c_str(),
                                "Assertion Failed",
                                MB_TASKMODAL | MB_ICONHAND | MB_ABORTRETRYIGNORE | MB_SETFOREGROUND);
        if (nCode == IDABORT) {
            raise(SIGABRT);
            exit(3);
        }

        if (nCode == IDRETRY) {
            DebugBreak();
            return;
        }
    }
#else
    void AssertionFailed(const char* Message, const char* Func, const char* File, int Line) {
        std::string FileName(File);
        auto LastSlash = FileName.find_last_of("/\\");
        if (LastSlash != std::string::npos) {
            FileName.erase(0, LastSlash + 1);
        }

        auto Str = ::GSZHU::FormatString("Assertion failed in ", Func, "(), (", FileName, ", ", Line, "): ", Message);
        std::cerr << Str << std::endl;
    }
#endif
}