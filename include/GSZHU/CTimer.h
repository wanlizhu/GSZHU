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
            : m_stopped(true), m_stopping(false) {}
        CTimer(const CTimer& timer) { m_stopped = timer.m_stopped.load(); m_stopping = timer.m_stopping.load(); }
        virtual ~CTimer() { Stop(); }

        void StartTimer(int interval, Callback callback) {
            if (!m_stopped) 
                return;

            m_stopped = false;

            std::thread([this, interval, callback](){
                while (!m_stopping) {
                    std::this_thread::sleep_for(std::chrono::milliseconds(interval));
                    callback();
                }

                std::lock_guard<std::mutex> locker(m_mutex);
                m_stopped = true;
                m_stopCV.notify_one();
            }).detach();
        }

        void Stop() {
            if (m_stopped || m_stopping) 
                return;

            m_stopping = true;
            std::unique_lock<std::mutex> locker(m_mutex);
            m_stopCV.wait(locker, [this](){ return m_stopped == true; });
            
            if (m_stopped) m_stopping = false;
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
        std::atomic<bool> m_stopped;
        std::atomic<bool> m_stopping;
        std::mutex m_mutex;
        std::condition_variable m_stopCV;
    };
}