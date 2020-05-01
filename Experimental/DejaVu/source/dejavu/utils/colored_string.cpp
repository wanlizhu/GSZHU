#include "colored_string.h"

#if defined(_WIN32)
#define NOMINMAX
#include <Windows.h>
#include <io.h>
#define isatty _isatty
#define fileno _fileno
#else
#include <unistd.h>
#endif

namespace djv
{
#if defined(_WIN32)
    struct SetupVirtualTerminal 
    {
        SetupVirtualTerminal()
        {
            auto enableVirtualTerminalFunc = [](DWORD handle){
                DWORD mode = 0;
                HANDLE console = GetStdHandle(handle);
                if (console == INVALID_HANDLE_VALUE) {
                    return;
                }

                GetConsoleMode(console, &mode);
                mode |= ENABLE_VIRTUAL_TERMINAL_PROCESSING;
                SetConsoleMode(console, mode);
            };

            enableVirtualTerminalFunc(STD_OUTPUT_HANDLE);
            enableVirtualTerminalFunc(STD_ERROR_HANDLE);
        }
    };
    static SetupVirtualTerminal setupVirtualTerminal;
#endif

    static const std::unordered_map<Color, std::string> skColorBeginTag = {
        { Color::Gray,    "\33[90m" },
        { Color::Red,     "\33[91m" },
        { Color::Green,   "\33[92m" },
        { Color::Yellow,  "\33[93m" },
        { Color::Blue,    "\33[94m" },
        { Color::Magenta, "\33[95m" }
    };
    static const std::string skColorEndTag = "\033[0m";

    inline bool is_terminal(const std::ostream& stream)
    {
        if (&stream == &std::cout && isatty(fileno(stdout)))
            return true;
        if (&stream == &std::cerr && isatty(fileno(stderr)))
            return true;
        return false;
    }

    std::string colored_string(
        Color color, 
        const std::string& str, 
        const std::ostream& stream
    )
    {
        return is_terminal(stream) ? (skColorBeginTag.at(color) + str + skColorEndTag) : str;
    }
}