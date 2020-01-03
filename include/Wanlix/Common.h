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
#include <condition_variable>
#include "Wanlix/Config.h"
#include "Wanlix/Flags.h"
#include "Wanlix/Macro.h"
#include "Wanlix/Object.h"

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
    using StringList = std::vector<std::string>;
    using Path = std::wstring;
    using ByteArray = std::vector<Byte>;
    
    template<typename _Value_>
    using StringMap = std::unordered_map<std::string, _Value_>;
    
    template<typename _Value_>
    using Array = std::vector<_Value_>;
    using ByteArray = std::vector<uint8_t>;
    using StringArray = std::vector<std::string>;
    
    template<typename _Key_, typename _Value_>
    using HashMap = std::unordered_map<_Key_, _Value_>;
    
    template<typename T>
    using SharedPtr = std::shared_ptr<T>;
    
    template<typename T>
    using UniquePtr = std::unique_ptr<T>;


    using Int2 = std::array<int32_t, 2>;
    using Int3 = std::array<int32_t, 3>;
    using Int4 = std::array<int32_t, 4>;
    using Uint2 = std::array<uint32_t, 2>;
    using Uint3 = std::array<uint32_t, 3>;
    using Uint4 = std::array<uint32_t, 4>;
    using Float2 = std::array<float, 2>;
    using Float3 = std::array<float, 3>;
    using Float4 = std::array<float, 4>;
    using Uv = Float2;
    using Uvw = Float3;
    using Color = Float4;
    using Quat = Float4;
    using Float3x3 = std::array<float, 9>;
    using Float4x4 = std::array<float, 16>;
    using Variant = std::variant<
        Bool, Byte, Uint, uint32_t, uint64_t, float, double,
        Int2, Int3, Int4, Uint2, Uint3, Uint4, Float2, Float3, Float4,
        Float3x3, Float4x4, Pvoid, String, StringList
    >;

    struct DataBlockFlags
    {
        enum {
            None = 0,
            IsArray = 1,
            IsOwner = (1 << 1),
        };
    };

    class DataBlock final
    {
    public:
        DataBlock() = default;
        DataBlock(Pvoid data, Size size, Uint flags = 0);
        DataBlock(DataBlock const& rhs);
        DataBlock(DataBlock&& rhs);
        ~DataBlock();
        DataBlock& operator=(DataBlock const& rhs);
        DataBlock& operator=(DataBlock&& rhs);

        Byte& operator[](int index);
        Byte const& operator[](int index) const;
        Bool operator==(DataBlock const& rhs) const;
        Bool operator!=(DataBlock const& rhs) const;

        Size  GetSize() const;
        Uint  GetFlags() const;
        Pvoid GetData();
        const void* GetData() const;

    private:
        void Free();

    private:
        Pvoid mData = nullptr;
        Size  mSize = 0;
        Uint  mFlags = 0;
    };

    class AABB final
    {
    public:
        AABB();
        AABB(Float3 const& min, Float3 const& max);

        Float3& GetMin();
        Float3& GetMax();
        Float3 const& GetMin() const;
        Float3 const& GetMax() const;

        Bool operator==(AABB const&) const;
        Bool operator!=(AABB const&) const;
        Bool Intersects(AABB const& box) const;
        Bool Contains(AABB const& box) const;
        void Extent(Float3 const& point);
        void Extent(AABB const& box);

    private:
        Float3 mMin;
        Float3 mMax;
    };

    template<typename T>
    inline Bool IsPowerOf2(T num) {
        Ui64 tmp = (Ui64)num;
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