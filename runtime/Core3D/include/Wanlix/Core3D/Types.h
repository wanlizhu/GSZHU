#pragma once

#include <cstdint>
#include <type_traits>
#include <cassert>
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
#include "Config.h"

namespace Wanlix
{
    using String     = std::string;
    using StringCRef = std::string const&;
    using StringList = std::vector<std::string>;
    template<typename T>
    using StringMap  = std::unordered_map<std::string, T>;

    using WString     = std::wstring;
    using WStringCRef = std::wstring const&;
    using WStringList = std::vector<std::wstring>;
    template<typename T>
    using WStringMap  = std::unordered_map<std::wstring, T>;

    struct Range 
    {
        uint32_t offset = 0;
        uint32_t size = 0;

        static Range Null();
        static Range All();

        Range() noexcept = default;
        Range(uint32_t offset, uint32_t size) noexcept
            : offset(offset), size(size)
        {}
        Range(StringCRef str) noexcept
            : offset(0), size((uint32_t)str.size())
        {}
        Range(WStringCRef str) noexcept
            : offset(0), size((uint32_t)str.size())
        {}
        Range(const std::initializer_list<uint32_t>& init) noexcept 
            : offset(*(init.begin())), size(*(init.begin()+1))
        {
            assert(init.size() == 2);
        }

        bool IsEmpty() const;
        bool IsAll() const;
        Range operator+(uint32_t count) const;
        Range operator-(uint32_t count) const;
        bool operator==(const Range& other) const;
        bool operator!=(const Range& other) const;
    };

    struct Offset
    {
        int x = 0;
        int y = 0;
        int z = 0;

        static Offset Origin();

        Offset() noexcept = default;
        Offset(int x, int y, int z = 0) noexcept
            : x(x), y(y), z(z)
        {}
        Offset(const std::initializer_list<uint32_t>& init) noexcept
            : Offset()
        {
            assert(init.size() <= 3);
            std::copy(init.begin(), init.end(), &x);
        }

        bool IsOrigin() const;
        Offset operator+(const Offset& other) const;
        Offset operator-(const Offset& other) const;
        bool operator==(const Offset& other) const;
        bool operator!=(const Offset& other) const;
    };

    struct Extent
    {
        uint32_t width = 0;
        uint32_t height = 0;
        uint32_t depth = 0;

        static Extent Null();
        static Extent All();

        Extent() noexcept = default;
        Extent(uint32_t width, uint32_t height, uint32_t depth = 0) noexcept
            : width(width), height(height), depth(depth)
        {}
        Extent(const std::initializer_list<uint32_t>& init) noexcept 
            : Extent()
        {
            assert(init.size() <= 3);
            std::copy(init.begin(), init.end(), &width);
        }

        bool IsEmpty() const;
        bool IsAll() const;
        Extent operator+(const Extent& other) const;
        Extent operator-(const Extent& other) const;
        bool operator==(const Extent& other) const;
        bool operator!=(const Extent& other) const;
    };

    struct Region
    {
        Offset offset;
        Extent extent;

        static Region Null() {
            return Region(Offset::Null(), Extent::Null());
        }

        Region() = default;
        Region(const Offset& offset, const Extent& extent)
            : offset(offset), extent(extent)
        {}

        bool IsEmpty() const;
        Region operator+(const Extent& ext) const;
        Region operator-(const Extent& ext) const;
        bool operator==(const Region& other) const;
        bool operator!=(const Region& other) const;
    };
}