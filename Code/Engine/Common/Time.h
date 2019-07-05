#pragma once
#include "Config.h"
#include <chrono>
#include <stdint.h>
#include <string_view>
#include <unordered_map>

namespace ZHU
{
    class ZHU_API Time final
    {
    public:
        Time() = default;

        static Time        Now();
        static std::string GetDateTime(std::string_view format = "%Y-%m-%d %H:%M:%S");
        bool operator==(const Time& other) const;
        bool operator!=(const Time& other) const;
        bool operator< (const Time& other) const;
        bool operator<=(const Time& other) const;
        bool operator> (const Time& other) const;
        bool operator>=(const Time& other) const;
        Time operator- () const;
        friend Time operator+(const Time& left, const Time& right);
        friend Time operator-(const Time& left, const Time& right);
        friend Time operator*(const Time& left, const float& right);
        friend Time operator*(const Time& left, const int64_t& right);
        friend Time operator*(const float& left, const Time& right);
        friend Time operator*(const int64_t& left, const Time& right);
        friend Time operator/(const Time& left, const float& right);
        friend Time operator/(const Time& left, const int64_t& right);
        friend double operator/(const Time& left, const Time& right);
        Time& operator+=(const Time& other);
        Time& operator-=(const Time& other);
        Time& operator*=(const float& other);
        Time& operator*=(const int64_t& other);
        Time& operator/=(const float& other);
        Time& operator/=(const int64_t& other);

        template<typename T, typename PERIOD>
        operator std::chrono::duration<T, PERIOD>() const { 
            return std::chrono::duration_cast<std::chrono::duration<T, PERIOD>>(mMicroseconds);
        }


        template<typename T = float>
        static constexpr Time Seconds(const T& seconds) {
            return Time(std::chrono::duration<T>(seconds));
        }

        template<typename T = int64_t>
        static constexpr Time Milliseconds(const T& milli) {
            return Time(std::chrono::duration<T, std::milli>(milli));
        }

        template<typename T = int64_t>
        static constexpr Time Microiseconds(const T & micro) {
            return Time(std::chrono::duration<T, std::micro>(micro));
        }

        template<typename T = float>
        inline T AsSeconds() const {
            return static_cast<T>(mMicroseconds.count() / static_cast<T>(1000000));
        }

        template<typename T = int64_t>
        inline T AsMilliseconds() const
        {
            return static_cast<T>(mMicroseconds.count()) / static_cast<T>(1000);
        }

        template<typename T = int64_t>
        inline T AsMicroseconds() const
        {
            return static_cast<T>(mMicroseconds.count());
        }

    private:
        template<typename PERIOD, typename T = int64_t>
        constexpr Time(const std::chrono::duration<T, PERIOD> & duration)
            : mMicroseconds(std::chrono::duration_cast<std::chrono::microseconds>(duration))
        {}

    private:
        static const std::chrono::time_point<std::chrono::high_resolution_clock> smStartPoint;
        std::chrono::microseconds mMicroseconds;
    };
}

namespace std
{
    template<>
    struct hash<ZHU::Time>
    {
        std::size_t operator()(const ZHU::Time& time) const 
        {
            return std::hash<size_t>()(time.AsMicroseconds<size_t>());
        }
    };
}