#pragma once

#include "common/Config.h"
#include <sstream>
#include <iomanip>
#include <string>
#include <memory>
#include <assert.h>
#include <iostream>
#include <atomic>
#include <typeinfo>
#include <codecvt>


namespace NTGS {

    template<typename T>
    std::size_t Hash(const std::vector<T>& vec) {
        std::size_t seed = vec.size();
        for (auto& i : vec)
            seed ^= std::hash<T>()(i) + 0x9e3779b9 + (seed << 6) + (seed >> 2);
        return seed;
    }

    template<typename T, size_t SizeOfArray>
    constexpr size_t CountOf(T(&array)[SizeOfArray]) {
        return SizeOfArray;
    }

    template<typename DST, typename SRC>
    DST Convert(const SRC& src) {}

    template<>
    std::wstring Convert<std::wstring, std::string>(const std::string& src);

    class NonCopyable {
    protected:
        constexpr NonCopyable() = default;
        ~NonCopyable() = default;
        NonCopyable(const NonCopyable&) = delete;
        NonCopyable& operator=(const NonCopyable&) = delete;
    };

    template<typename T>
    class Singleton : NonCopyable {
    public:
        static T& GetInstance() {
            std::call_once(smOnce, [&](){ smGlobalInstance.reset(new T()); });
            return *GlobalInstance.get();
        }

    private:
        static std::unique_ptr<T> smGlobalInstance;
        static std::once_flag smOnce;
    };

    template<typename T>
    std::unique_ptr<T> Singleton<T>::smGlobalInstance;

}