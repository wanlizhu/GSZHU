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
#include <optional>
#include <thread>
#include <mutex>
#include <any>
#include <queue>
#include <deque>
#include <variant>
#include <typeindex>
#include "glm/glm.hpp"
#include "Config.h"

#define Color_RED     glm::ivec4(255, 0, 0, 255)
#define Color_GREEN   glm::ivec4(0, 255, 0, 255)
#define Color_BLUE    glm::ivec4(0, 0, 255, 255)
#define Color_WHITE   glm::ivec4(255, 255, 255, 255)
#define Color_BLACK   glm::ivec4(0, 0, 0, 255)
#define Color_CYAN    glm::ivec4(0, 255, 255, 255)
#define Color_AutoCAD glm::ivec4(33, 40, 48, 255)

namespace Wanli
{
    using Uint = uint32_t;
    using Uint8 = uint8_t;
    using Uint16 = uint16_t;
    using Uint64 = uint64_t;
    using Byte = uint8_t;
    using PCCH = const char*;
    using Path = std::filesystem::path;
    using Clock = std::chrono::system_clock;
    using TimePoint = std::chrono::system_clock::time_point;
    using Duration = std::chrono::system_clock::duration;
    using Second = std::chrono::duration<double>;
    using MilliSecond = std::chrono::duration<double, std::milli>;
    using FileTime = std::filesystem::file_time_type;

    using String = std::string;
    using ByteArray = std::vector<uint8_t>;
    using PCCHArray = std::vector<const char*>;
    using StringArray = std::vector<std::string>;
    using StringMap = std::unordered_map<std::string, std::string>;
    using StringArrayMap = std::unordered_map<std::string, std::vector<std::string>>;

    //using Color = glm::ivec4;

    template<typename T>
    using Array = std::vector<T>;

    template<typename K,
             typename V, 
             typename _Hash_ = std::hash<K>, 
             typename _Equal_ = std::equal_to<K>>
    using HashMap = std::unordered_map<K, V, _Hash_, _Equal_>;

    template<typename T>
    using SharedPtr = std::shared_ptr<T>;

    template<typename T>
    using WeakPtr = std::weak_ptr<T>;

    template<typename T>
    using UniquePtr = std::unique_ptr<T>;

    template<typename T>
    using Optional = std::optional<T>;

    struct Version
    {
        Uint8 major = 0;
        Uint8 minor = 0;
        Uint8 patch = 0;
    };
}