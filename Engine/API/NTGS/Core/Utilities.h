#pragma once

#include <NTGS/Core/Config.h>
#include <sstream>
#include <iomanip>
#include <string>
#include <memory>
#include <assert.h>
#include <iostream>
#include <atomic>
#include <typeinfo>

#ifdef _DEBUG
    #define ASSERTION_FAILED(Message, ...) \
    do {\
        auto Str = NTGS::Concat(Message, ##__VA_ARGS__);\
        NTGS::DebugOutput::AssertionFailed(Str.c_str(), __FUNCTION__, __FILE__, __LINE__);\
    } while(false)

    #define CHECK(Expr, Message, ...) \
    do {\
        if (!(Expr)) { ASSERTION_FAILED(Message, ##__VA_ARGS__);}\
    } while(false)

    #define CHECK_DYNAMIC_TYPE(DstType, SrcPtr) \
    do {\
        CHECK(SrcPtr == nullptr || dynamic_cast<DstType*>(SrcPtr) != nullptr, \
              "Dynamic type cast failed. Source Type: '",\
              typeid(*SrcPtr).name(), "' Dest Type: '", typeid(DstType).name(), "'"); \
    } while (false)

    #define UNEXPECTED ASSERTION_FAILED

#define LOG_AND_THROW(...) NTGS::DebugOutput::OnError<true>(__FUNCTION__, __FILE__, __LINE__, ##__VA_ARGS__)
#else
    #define ASSERTION_FAILED(...) do {} while(false)
    #define CHECK(...) do {} while(false)
    #define CHECK_DYNAMIC_TYPE(...) do {} while(false)
    #define UNEXPECTED ASSERTION_FAILED
#endif

namespace NTGS {

    template<typename EnumType>
    using UnderlyingEnumType = typename std::underlying_type<EnumType>::type;

#define DEFINE_FLAG_ENUM_OPERATORS(ENUMTYPE) \
extern "C++" {\
    inline           ENUMTYPE& operator |= (ENUMTYPE& a, ENUMTYPE b) throw() { return reinterpret_cast<ENUMTYPE&>(reinterpret_cast<UnderlyingEnumType<ENUMTYPE>&>(a) |= static_cast<UnderlyingEnumType<ENUMTYPE>>(b)); } \
    inline           ENUMTYPE& operator &= (ENUMTYPE& a, ENUMTYPE b) throw() { return reinterpret_cast<ENUMTYPE&>(reinterpret_cast<UnderlyingEnumType<ENUMTYPE>&>(a) &= static_cast<UnderlyingEnumType<ENUMTYPE>>(b)); } \
    inline           ENUMTYPE& operator ^= (ENUMTYPE& a, ENUMTYPE b) throw() { return reinterpret_cast<ENUMTYPE&>(reinterpret_cast<UnderlyingEnumType<ENUMTYPE>&>(a) ^= static_cast<UnderlyingEnumType<ENUMTYPE>>(b)); } \
    inline constexpr ENUMTYPE  operator |  (ENUMTYPE  a, ENUMTYPE b) throw() { return static_cast<ENUMTYPE> (static_cast<UnderlyingEnumType<ENUMTYPE>>(a) |  static_cast<UnderlyingEnumType<ENUMTYPE>>(b)); } \
    inline constexpr ENUMTYPE  operator &  (ENUMTYPE  a, ENUMTYPE b) throw() { return static_cast<ENUMTYPE> (static_cast<UnderlyingEnumType<ENUMTYPE>>(a) &  static_cast<UnderlyingEnumType<ENUMTYPE>>(b)); } \
    inline constexpr ENUMTYPE  operator ^  (ENUMTYPE  a, ENUMTYPE b) throw() { return static_cast<ENUMTYPE> (static_cast<UnderlyingEnumType<ENUMTYPE>>(a) ^  static_cast<UnderlyingEnumType<ENUMTYPE>>(b)); } \
    inline constexpr ENUMTYPE  operator ~  (ENUMTYPE  a)             throw() { return static_cast<ENUMTYPE> (~static_cast<UnderlyingEnumType<ENUMTYPE>>(a)); } \
}

    // String formatting
    template<typename StreamType>
    void AppendToStream(StreamType& Stream) {}

    template<typename StreamType, typename ArgType>
    inline void AppendToStream(StreamType& Stream, const ArgType& Arg) { Stream << Arg; }

    template<typename StreamType, typename FirstArgType, typename... RestArgsType>
    void AppendToStream(StreamType& Stream, const FirstArgType& FirstArg, const RestArgsType&... RestArgs) { Concat(Stream, FirstArg); Concat(Stream, RestArgs); }

    template<typename... ArgsType>
    std::string Concat(const ArgsType&... Args) {
        std::stringstream Stream;
        AppendToStream(Stream, Args...);
        return Stream.str();
    }

    class DebugOutput {
    public:
        enum class Severity : int32_t {
            Fatal = 0,
            Error,
            Warning,
            Info
        };
        using MessageCallback = void(*)(Severity _Severity, const char* Msg, const char* Func, const char* File, int Line);

        static void SetMessageCallback(MessageCallback _Callback) { sm_Callback = _Callback; }
        static MessageCallback GetMessageCallback() { return sm_Callback; }
        static void AssertionFailed(const char* Msg, const char* Func, const char* File, int Line);
        static void Message(Severity _Severity, const char* Msg, const char* Func, const char* File, int Line);

        template<bool ThrowException, typename... ArgsType>
        static void OnError(const char* Func, const char* Path, int Line, const ArgsType&... Args) {
            std::string FileName(Path);
            auto LastSlash = FileName.find_last_of("/\\");
            if (LastSlash != std::string::npos)
                FileName.erase(0, LastSlash + 1);

            std::string Msg = Concat(Args...);
            if (sm_Callback) {
                sm_Callback(ThrowException ? Severity::Fatal : Severity::Error, Msg, Func, FileName.c_str(), Line);
            }
            else {
                std::cerr << (ThrowException ? "Fatal Error" : "Error") 
                          << " in " << Func << "() (" << FileName.c_str() << ", " << Line << "): " 
                          << Msg << std::endl;
            }

            if (ThrowException) throw std::runtime_error(Msg);
        }

    private:
        static MessageCallback sm_Callback;
    };

    class NonCopyable {
    protected:
        NonCopyable() = default;

    private:
        NonCopyable(const NonCopyable&) = default;
        NonCopyable& operator=(const NonCopyable&) = default;
    };

    template<typename T>
    class Singleton : NonCopyable {
    public:
        static T* Instance() {
            std::call_once(sm_Once, [&](){ sm_GlobalInstance.reset(new T()); });
            return sm_GlobalInstance.get();
        }

    private:
        static std::unique_ptr<T> sm_GlobalInstance;
        static std::once_flag sm_Once;
    };

    template<typename T>
    std::unique_ptr<T> Singleton<T>::sm_GlobalInstance;

    template<typename ReturnType = const char*, typename T = void>
    inline ReturnType ToStr(const T& Value) { static_assert(false); }

    template<typename T>
    class IDGenerator {
    public:
        static inline SIZE GetID() const { return sm_GlobalCounter++; }

    private:
        static std::atomic<SIZE> sm_GlobalCounter;
    };

    template<typename T>
    std::atomic<SIZE> IDGenerator<T>::sm_GlobalCounter = 0;
}