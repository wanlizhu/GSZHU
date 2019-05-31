#pragma once

#include <chrono>
#include <atomic>
#include <thread>
#include <condition_variable>
#include <mutex>
#include <functional>

namespace GSZHU {
    class CTimer {
    public:
        typedef std::function<void()> Callback;
        struct MilliSeconds {
            std::chrono::high_resolution_clock::time_point m_time;

            inline MilliSeconds() {
                m_time = std::chrono::high_resolution_clock::now();
            }

            inline double operator-(const MilliSeconds& ms) const { 
                std::chrono::duration<double, std::milli> elapsed = m_time - ms.m_time;
                return elapsed.count();
            }
        };

        CTimer()
            : mStopped(true)
            , mStopping(false) 
        {}
        CTimer(const CTimer& timer) {
            mStopped = timer.mStopped.load();
            mStopping = timer.mStopping.load();
        }
        virtual ~CTimer() { Stop(); }

        void StartTimer(int interval, Callback callback) {
            if (!mStopped) 
                return;

            mStopped = false;

            std::thread([this, interval, callback](){
                while (!mStopping) {
                    std::this_thread::sleep_for(std::chrono::milliseconds(interval));
                    callback();
                }

                std::lock_guard<std::mutex> locker(mMutex);
                mStopped = true;
                mStopCV.notify_one();
            }).detach();
        }

        void Stop() {
            if (mStopped || mStopping) 
                return;

            mStopping = true;
            std::unique_lock<std::mutex> locker(mMutex);
            mStopCV.wait(locker, [this](){ return mStopped == true; });
            
            if (mStopped) mStopping = false;
        }

        template<typename Function, typename... Args>
        void SyncWait(int after, Function&& func, Args&&... args) {
            auto callback(std::bind(std::forward<Function>(func), std::forward<Args>(args)...));
            
            std::this_thread::sleep_for(std::chrono::milliseconds(after));
            callback();
        }

        template<typename Function, typename... Args>
        void AsyncWait(int after, Function&& func, Args&&... args) {
            auto callback(std::bind(std::forward<Function>(func), std::forward<Args>(args)...));
            
            std::thread([after, callback](){
                std::this_thread::sleep_for(std::chrono::milliseconds(after));
                callback();
            }).detach();
        }

    private:
        std::atomic<bool> mStopped;
        std::atomic<bool> mStopping;
        std::mutex mMutex;
        std::condition_variable mStopCV;
    };
}