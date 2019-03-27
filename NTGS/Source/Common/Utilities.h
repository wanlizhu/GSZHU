#pragma once

#include "Common/Config.h"
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

    template<typename...> struct Hash;

    template<typename T>
    struct Hash<T> : public std::hash<T> {
        using std::hash<T>::hash;
    };

    template<typename T, typename... REST>
    struct Hash<T, REST...> {
        inline std::size_t operator()(const T& v, const REST&... rest) {
            std::size_t seed = Hash<REST...>()(rest...);
            seed ^= Hash<T>()(v) + 0x9e3779b9 + (seed << 6) + (seed >> 2);
            return seed;
        }
    };

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
        static T* GetInstance() {
            std::call_once(smOnce, [&](){ smGlobalInstance.reset(new T()); });
            return GlobalInstance.get();
        }

    private:
        static std::unique_ptr<T> smGlobalInstance;
        static std::once_flag smOnce;
    };

    template<typename T>
    std::unique_ptr<T> Singleton<T>::smGlobalInstance;

}