#pragma once

#include <thread>
#include <mutex>
#include <condition_variable>
#include <chrono>
#include <atomic>
#include <memory>
#include <future>
#include "utils/preprocess.h"

namespace djv
{
    class DJV_API Timer
    {
    public:
        using Interval = std::chrono::milliseconds;
        using Callback = std::function<void()>;

        Timer() = default;
        Timer(Interval interval, const Callback& callback, bool repeat);
        
        void start();
        void stop();

        bool is_running() const;
        void set_interval(Interval interval);
        void set_repeat(bool value);
        void set_callback(const Callback& callback);

    private:
        std::atomic_bool mRunning = false;
        std::atomic_bool mRepeat = false;

        Interval mInterval = Interval(0);
        Callback mCallback = nullptr;
        std::shared_ptr<std::thread> mThread;
    };
}