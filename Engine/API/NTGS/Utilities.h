#pragma once

#include <sstream>
#include <iomanip>
#include <string>
#include <memory>
#include <assert.h>

namespace NTGS {

    // String formatting
    template<typename StreamType>
    void AppendToStream(StreamType& ss) {}

    template<typename StreamType, typename ArgType>
    inline void AppendToStream(StreamType& ss, const ArgType& arg) { ss << arg; }

    template<typename StreamType, typename FirstArgType, typename... RestArgsType>
    void AppendToStream(StreamType& ss, const FirstArgType& firstArg, const RestArgsType&... restArgs) { Concat(ss, firstArg); Concat(ss, restArgs); }

    struct MemorySize {
        double size = 0.0;
        MemorySize(double _size = 0.0) : size(_size) {}
    };

    template<typename SteamType>
    void AppendToStream<StreamType, MemorySize>(StreamType& ss, const MemorySize& size) {
        if (size.size >= (1 << 30)) 
            ss << std::fixed << std::setprecision(2) << size.size / double{1 << 30} << " GB";
        else if (size.size >= (1 << 20)) 
            ss << std::fixed << std::setprecision(2) << size.size / double{1 << 20} << " MB";
        else if (size.size >= (1 << 10))
            ss << std::fixed << std::setprecision(2) << size.size / double{1 << 10} << " KB";
        else
            ss << std::fixed << std::setprecision(2) << size.size << " B";
    }

    template<typename... ArgsType>
    std::string Concat(const ArgsType&... args) {
        std::stringstream ss;
        AppendToStream(ss, args...);
        return ss.str();
    }

    class NonCopyable {
    protected:
        NonCopyable() = default;

    private:
        NonCopyable(const NonCopyable&);
        NonCopyable& operator=(const NonCopyable&);
    };

    template<typename T>
    class Singleton : NonCopyable {
    public:
        static T* Instance() {
            std::call_once(sm_once, [&](){ sm_instance.reset(new T()); });
            return sm_instance.get();
        }

    private:
        static std::unique_ptr<T> sm_instance;
        static std::once_flag sm_once;
    };

    template<typename T>
    std::unique_ptr<T> Singleton<T>::sm_instance;

    template<ReturnType = const char*, typename T>
    inline ReturnType ToStr(const T& value) { static_assert(false); }
}