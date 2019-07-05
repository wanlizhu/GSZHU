#include "ZHUENGINE_PCH.h"
#include "Time.h"
#include <iomanip>
#include <iostream>

namespace ZHU
{
    const std::chrono::time_point<std::chrono::high_resolution_clock> Time::smStartPoint
        = std::chrono::high_resolution_clock::now();

    Time Time::Now()
    {
        return std::chrono::duration_cast<std::chrono::microseconds>(
                   std::chrono::high_resolution_clock::now() - smStartPoint
               );
    }

    std::string Time::GetDateTime(std::string_view format)
    {
        auto now{ std::chrono::system_clock::now() };
        auto timeT{ std::chrono::system_clock::to_time_t(now) };

        std::stringstream ss;
        ss << std::put_time(std::localtime(&timeT), format.data());
        return ss.str();
    }

    bool Time::operator==(const Time& other) const { return mMicroseconds == other.mMicroseconds; }
    bool Time::operator!=(const Time& other) const { return mMicroseconds != other.mMicroseconds; }
    bool Time::operator< (const Time& other) const { return mMicroseconds <  other.mMicroseconds; }
    bool Time::operator<=(const Time& other) const { return mMicroseconds <= other.mMicroseconds; }
    bool Time::operator> (const Time& other) const { return mMicroseconds >  other.mMicroseconds; }
    bool Time::operator>=(const Time& other) const { return mMicroseconds >= other.mMicroseconds; }
    Time Time::operator- () const { return -mMicroseconds; }
    
    Time   operator+(const Time& left,    const Time&    right) { return left.mMicroseconds + right.mMicroseconds; }
    Time   operator-(const Time& left,    const Time&    right) { return left.mMicroseconds - right.mMicroseconds; }
    Time   operator*(const Time& left,    const float&   right) { return left.mMicroseconds * right; }
    Time   operator*(const Time& left,    const int64_t& right) { return left.mMicroseconds * right; }
    Time   operator*(const float& left,   const Time&    right) { return left * right.mMicroseconds; }
    Time   operator*(const int64_t& left, const Time&    right) { return left * right.mMicroseconds; }
    Time   operator/(const Time& left,    const float&   right) { return left.mMicroseconds / right; }
    Time   operator/(const Time& left,    const int64_t& right) { return left.mMicroseconds / right; }
    double operator/(const Time& left,    const Time&    right) { return left.mMicroseconds / right.mMicroseconds; }

    Time& Time::operator+=(const Time& other)    { *this = *this + other; return *this; } 
    Time& Time::operator-=(const Time& other)    { *this = *this - other; return *this; }
    Time& Time::operator*=(const float& other)   { *this = *this * other; return *this; }
    Time& Time::operator*=(const int64_t& other) { *this = *this * other; return *this; }
    Time& Time::operator/=(const float& other)   { *this = *this / other; return *this; }
    Time& Time::operator/=(const int64_t& other) { *this = *this / other; return *this; }
}