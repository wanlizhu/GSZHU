#pragma once

#include <string>
#include <vector>
#include <cstdio>
#include <iostream>
#include <unordered_map>
#include "utils/preprocess.h"

namespace djv
{
    enum class Color
    {
        Gray,
        Red,
        Green,
        Yellow,
        Blue,
        Magenta,
    };

    std::string DJV_API colored_string(
        Color color, 
        const std::string& str, 
        const std::ostream& stream = std::cout
    );
}