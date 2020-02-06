#pragma once

#include <string>
#include <vector>
#include <unordered_map>
#include <array>
#include <memory>
#include <algorithm>
#include <functional>
#include <filesystem>
#include <chrono>
#include "Config.h"

namespace Wanli
{
    using Uint = uint32_t;
    using Uint8 = uint8_t;
    using Uint16 = uint16_t;
    using Uint64 = uint64_t;
    using Byte = uint8_t;
    using Path = std::filesystem::path;
    using Clock = std::chrono::high_resolution_clock;
    using TimePoint = std::chrono::high_resolution_clock::time_point;
    using Duration = std::chrono::high_resolution_clock::duration;

    using String = std::string;
    using ByteArray = std::vector<uint8_t>;
    using PCCHArray = std::vector<const char*>;
    using StringArray = std::vector<std::string>;
    using StringMap = std::unordered_map<std::string, std::string>;
    using StringArrayMap = std::unordered_map<std::string, std::vector<std::string>>;

    template<typename T>
    using Array = std::vector<T>;

    template<typename K, typename V>
    using HashMap = std::unordered_map<K, V>;

    template<typename T>
    using SharedPtr = std::shared_ptr<T>;

    template<typename T>
    using WeakPtr = std::weak_ptr<T>;

    template<typename T>
    using UniquePtr = std::unique_ptr<T>;

    struct Version
    {
        Uint8 major = 0;
        Uint8 minor = 0;
        Uint8 patch = 0;
    };
}