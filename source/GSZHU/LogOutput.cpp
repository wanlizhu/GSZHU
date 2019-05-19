#include <GSZHU/LogOutput.h>

namespace GSZHU {
    LOG_MESSAGE_CALLBACK GLOBAL_LogMessageCallback = nullptr;

    void SetLogCallback(LOG_MESSAGE_CALLBACK Callback) {
        GLOBAL_LogMessageCallback = Callback;
    }
}