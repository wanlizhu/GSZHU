#pragma once

#include <sstream>
#include <iomanip>

namespace GSZHU {
    template<typename STREAM>
    void FormatStringSS(STREAM& Val) {}

    template<typename STREAM, typename T>
    void FormatStringSS(STREAM& Stream, const T& Arg) {
        Stream << Arg;
    }

    template<typename STREAM, typename FIRST, typename... REST>
    void FormatStringSS(STREAM& Stream, const FIRST& First, const REST&... Rest) {
        FormatString(Stream, First);
        FormatString(Stream, Rest...);
    }

    template<typename... ARGS>
    std::string FormatString(const ARGS&... Args) {
        std::stringstream Stream;
        FormatStringSS(Stream, Args...);
        return Stream.str();
    }
}