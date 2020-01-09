#pragma once

#include <cstdint>
#include <type_traits>
#include <cassert>
#include <array>
#include <algorithm>
#include <unordered_map>
#include <memory>
#include <typeinfo>
#include <typeindex>
#include <functional>
#include <string>
#include <vector>
#include <queue>
#include <deque>
#include <list>
#include <codecvt>
#include <chrono>
#include <filesystem>
#include <fstream>
#include <sstream>
#include <iostream>
#include <any>
#include <tuple>
#include <variant>
#include <thread>
#include <mutex>
#include <optional>
#include <condition_variable>
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/glm.hpp>
#include <glm/ext.hpp>

namespace Wanlix
{
    using Int8 = int8_t;
    using Int16 = int16_t;
    using Int = int32_t;
    using Int32 = int32_t;
    using Int64 = int64_t;
    using Bool = bool;
    using Byte = uint8_t;
    using Uint8 = uint8_t;
    using Uint16 = uint16_t;
    using Uint = uint32_t;
    using Uint32 = uint32_t;
    using Uint64 = uint64_t;
    using Size = size_t;
    using Pvoid = void*;
    using Wstring = std::wstring;
    using String = std::string;
    using Path = std::wstring;
    
    template<typename _Value_>
    using StringMap = std::unordered_map<std::string, _Value_>;
    
    template<typename _Value_>
    using Array = std::vector<_Value_>;
    using ByteArray = std::vector<uint8_t>;
    using ByteArrayPtr = std::shared_ptr<ByteArray>;
    using StringArray = std::vector<std::string>;
    using StringArrayPtr = std::shared_ptr<StringArray>;

    template<typename _Key_, typename _Value_>
    using HashMap = std::unordered_map<_Key_, _Value_>;
    
    template<typename T>
    using SharedPtr = std::shared_ptr<T>;

    template<typename T>
    using WeakPtr = std::weak_ptr<T>;
    
    template<typename T>
    using UniquePtr = std::unique_ptr<T>;

    template<typename T>
    using Optional = std::optional<T>;

    using Int2 = glm::ivec2;
    using Int3 = glm::ivec3;
    using Int4 = glm::ivec4;
    using Uint2 = glm::uvec2;
    using Uint3 = glm::uvec3;
    using Uint4 = glm::uvec4;
    using Float2 = glm::vec2;
    using Float3 = glm::vec3;
    using Float4 = glm::vec4;
    using Quaternion = glm::fquat;
    using Float2x2 = glm::mat2;
    using Float3x3 = glm::mat3;
    using Float4x4 = glm::mat4;

    struct DataBlockFlags
    {
        enum {
            None = 0,
            IsArray = 1,
            IsOwner = (1 << 1),
        };
    };

    template<typename T>
    inline Bool IsPowerOf2(T num) {
        Uint64 tmp = (Ui64)num;
        return (tmp & (tmp - 1)) == 0;
    }

    template<typename T>
    inline Bool Equals(T a, T b, T eps = T(0)) {
        if constexpr (std::is_floating_point_v<T>) {
            if (eps == T(0))
                eps = std::numeric_limits<T>::epsilon() * 100;
            return std::abs(a - b) < eps;
        }
        else {
            return a == b;
        }
    }
}