#pragma once

#include <array>
#include <vector>
#include <string>
#include <map>
#include <unordered_map>
#include <algorithm>
#include <memory>
#include <numeric>
#include <stdint.h>
#include <math.h>
#include <atomic>
#include <stdint.h>

#ifdef GSZHU_EXPORT_SHARED
#define GSZHU_API  __declspec(dllexport)
#elif defined(GSZHU_IMPORT_SHARED)
#define GSZHU_API __declspec(dllimport)
#else
#define GSZHU_API 
#endif

namespace GSZHU {
    typedef std::string STRING;
    typedef std::wstring WSTRING;
    typedef std::array<int, 2> INT2;
    typedef std::array<int, 3> INT3;
    typedef std::array<int, 4> INT4;
    typedef std::vector<std::string> STRING_LIST;
    typedef std::unordered_map<std::string, std::string> STRING_MAP;

    template<typename K, typename V>
    using HASH_MAP = std::unordered_map<K, V>;
}