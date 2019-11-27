#pragma once

#include <exception>
#include <stdexcept>
#include "Macro.h"

namespace Wanlix
{
    class UniqueNameExists : public std::runtime_error
    {
    public:
        UniqueNameExists() : std::runtime_error("") {}
        UniqueNameExists(const std::string& msg) : std::runtime_error(msg.c_str()) {}
        UniqueNameExists(const char* msg) : std::runtime_error(msg) {}
    };
}