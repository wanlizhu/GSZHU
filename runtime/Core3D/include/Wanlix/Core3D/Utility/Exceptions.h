#pragma once

#include <exception>
#include <stdexcept>
#include "Wanlix/Core3D/Types.h"
#include "Macro.h"

namespace Wanlix
{
    class UniqueNameExists : public std::runtime_error
    {
    public:
        UniqueNameExists() : std::runtime_error("") {}
        UniqueNameExists(StringCRef msg)  : std::runtime_error(msg.c_str()) {}
        UniqueNameExists(const char* msg) : std::runtime_error(msg) {}
    };
}