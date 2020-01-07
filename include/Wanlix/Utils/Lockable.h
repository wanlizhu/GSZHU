#pragma once

#include <mutex>

namespace Wanlix
{
    template<typename T>
    class Lockable
    {
    public:
        constexpr Lockable()
            : mMutex(std::make_shared<std::mutex>())
        {}

        constexpr Lockable(const T& data)
            : Lockable()
            , mData(data)
        {}

        template<typename... _Args_>
        constexpr Lockable(_Args_&& ... args)
            : Lockable()
            , mData(std::forward<_Args_>(args)...)
        {}

        inline T& GetData() noexcept {
            return mData;
        }

        inline const T& GetData() const noexcept {
            return mData;
        }

        inline std::mutex& GetMutex() const {
            return *mMutex.get();
        }

    private:
        T mData;
        mutable std::shared_ptr<std::mutex> mMutex;
    };
}