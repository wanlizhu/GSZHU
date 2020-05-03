#include "timer.h"

namespace djv
{
    Timer::Timer(
        Interval interval, 
        const Callback& callback, 
        bool repeat
    )
        : mInterval(interval)
        , mCallback(callback)
        , mRepeat(repeat)
    {}

    void Timer::start()
    {
        if (is_running())
            return;

        mRunning = true;
        mThread = std::make_shared<std::thread>([&](){
            while (is_running()) {
                std::this_thread::sleep_for(mInterval);
                if (is_running() && mCallback) {
                    auto future = std::async(std::launch::async, mCallback);
                }
                if (!mRepeat) {
                    break;
                }
            }
        });
    }

    void Timer::stop()
    {
        mRunning = false;
        if (mThread->joinable()) {
            mThread->join();
            mThread = nullptr;
        }
    }

    bool Timer::is_running() const
    {
        return mRunning;
    }

    void Timer::set_interval(Interval interval)
    {
        mInterval = interval;
    }

    void Timer::set_repeat(bool value)
    {
        mRepeat = value;
    }

    void Timer::set_callback(const Callback& callback)
    {
        mCallback = callback;
    }
}