#pragma once

#include "Common/Config.h"

namespace ZHU
{
    template<typename T>
    using BitSet = typename std::underlying_type<T>::type;
}