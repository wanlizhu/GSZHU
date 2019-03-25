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


namespace NTGS {

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