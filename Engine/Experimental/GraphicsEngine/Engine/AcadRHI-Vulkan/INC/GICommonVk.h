#pragma once

#include <memory>
#include <string>
#include <limits>
#include <numeric>
#include <array>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <vector>
#include <type_traits>
#include <typeindex>
#include <optional>
#include <variant>
#include <any>
#include <deque>
#include <atomic>
#include <functional>
#include <unordered_map>
#include <algorithm>
#include <filesystem>
#include <sstream>
#include <fstream>
#include <streambuf>
#include <stdint.h>
#include <assert.h>
#include "GIPlatformVk.h"

#define LOG_DEBUG_IF(expr, msg) if (expr) { LOG_DEBUG(msg); }
#define LOG_WARNING_IF(expr, msg) if (expr) { LOG_WARNING(msg); }
#define LOG_ERROR_IF(expr, msg) if (expr) { LOG_ERROR(msg); }
#define VK_CHECK(error) LOG_ERROR_IF(error != VK_SUCCESS, GIErrorDescVk(error))

#define LOAD_INSTANCE_FUNC(inst, name) reinterpret_cast<PFN_##name>(vkGetInstanceProcAddr(inst, #name))
#define LOAD_DEVICE_FUNC(device, name) reinterpret_cast<PFN_##name>(vkGetDeviceProcAddr(device, #name))

#define DECL_DEVICE_OBJECT(T) \
public: inline std::shared_ptr<T> shared_from_this() { \
    auto base = static_cast<GIDeviceObjectVk*>(this); \
    return std::static_pointer_cast<T>(base->shared_from_this()); \
} \
inline std::shared_ptr<const T> shared_from_this() const { \
    auto base = static_cast<const GIDeviceObjectVk*>(this); \
    return std::static_pointer_cast<const T>(base->shared_from_this()); \
} \
public: inline std::weak_ptr<T> weak_from_this() { \
    auto base = static_cast<GIDeviceObjectVk*>(this); \
    return std::static_pointer_cast<T>(base->weak_from_this().lock()); \
} \
inline std::weak_ptr<const T> weak_from_this() const { \
    auto base = static_cast<const GIDeviceObjectVk*>(this); \
    return std::static_pointer_cast<const T>(base->weak_from_this().lock()); \
} \
inline std::type_index GetTypeIndex() const { return typeid(T); } 

#define DECL_WEAK_DEVICE_OBJECT(T) \
public: inline std::shared_ptr<T> shared_from_this() { \
    auto base = static_cast<GIWeakDeviceObjectVk*>(this); \
    return std::static_pointer_cast<T>(base->shared_from_this()); \
} \
inline std::shared_ptr<const T> shared_from_this() const { \
    auto base = static_cast<const GIWeakDeviceObjectVk*>(this); \
    return std::static_pointer_cast<const T>(base->shared_from_this()); \
} \
public: inline std::weak_ptr<T> weak_from_this() { \
    auto base = static_cast<GIWeakDeviceObjectVk*>(this); \
    return std::static_pointer_cast<T>(base->weak_from_this().lock()); \
} \
inline std::weak_ptr<const T> weak_from_this() const { \
    auto base = static_cast<const GIWeakDeviceObjectVk*>(this); \
    return std::static_pointer_cast<const T>(base->weak_from_this().lock()); \
} \
inline std::type_index GetTypeIndex() const { return typeid(T); } 


namespace AutoCAD::Graphics::Engine
{
    template<typename T>
    using SharedPtr = std::shared_ptr<T>;

    template<typename T>
    using WeakPtr = std::weak_ptr<T>;

    template<typename T>
    using UniquePtr = std::unique_ptr<T>;

    using SET_ID = uint32_t;
    using BINDING_ID = uint32_t;
    using CACHE_INDEX = size_t;

    class GINonCopyable
    {
    public:
        GINonCopyable() = default;
        GINonCopyable(const GINonCopyable&) = delete;
        GINonCopyable(GINonCopyable&&) = default;
        GINonCopyable& operator=(const GINonCopyable&) = delete;
        GINonCopyable& operator=(GINonCopyable&&) = default;
    };

    const char* GIErrorDescVk(VkResult error);
    const char* GIFormatToStringVk(VkFormat format);
}

namespace std
{
    template <>
    struct hash<std::filesystem::path>
    {
        std::size_t operator()(const std::filesystem::path& key) const
        {
            return std::hash<std::wstring>()(key.wstring());
        }
    };
}