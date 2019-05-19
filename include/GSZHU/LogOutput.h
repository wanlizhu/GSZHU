#pragma once

#include <stdexcept>
#include <string>
#include <iostream>
#include <sstream>

namespace GSZHU {
    enum class LOG_SEVERITY : int {
        INFO = 0,
        WARNING,
        ERROR,
        FATAL_ERROR
    };

    using LOG_MESSAGE_CALLBACK = void(*)(LOG_SEVERITY Severity, const char* Message, const char* Func, const char* File, int Line);
    void SetLogCallback(LOG_MESSAGE_CALLBACK Callback);
}