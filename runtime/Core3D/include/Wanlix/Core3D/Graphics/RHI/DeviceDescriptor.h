#pragma once

#include <cstdint>
#include <string>
#include <memory>
#include <unordered_map>
#include <functional>

namespace Wanlix
{
    using DebugCallback = std::function<void(const std::string& type, const std::string& message)>;

    struct DeviceDescriptor
    {
        
    };
}