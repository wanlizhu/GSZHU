#include "ENGINE_PCH.h"
#include <Engine/Core/Timers.h>

namespace ZHU
{
    Timers::Timers()
    {
        std::unique_lock<std::mutex> lock(mMutex);
        mWorker = std::thread(std::bind(&Timers::Run, this));
    }
    
    Timers::~Timers()
    {
        {
            std::unique_lock<std::mutex> lock(mMutex);
            mQuit.store(true);
        }

        mCV.notify_all();
        mWorker.join();
    }

    void Timers::Update() 
    {}

    void Timers::Run()
    {
        std::unique_lock<std::mutex> lock(mMutex);

        while (!mQuit) {
            if (mTimers.empty()) {
                mCV.wait(lock);
            }
            else {
                std::sort(mTimers.begin(), mTimers.end(), 
                          [](const std::unique_ptr<Timer>& a, const std::unique_ptr<Timer>& b) {
                              return a->mNext < b->mNext;
                          }
                );

                auto& instance{ mTimers.front() };
                auto time{ Time::Now() };

                if (time >= instance->mNext) {
                    lock.unlock();
                    instance->mOnTick();
                    lock.lock();

                    instance->mNext += instance->mInterval;

                    if (instance->mRepeat) {
                        (*instance->mRepeat)--;

                        if (*instance->mRepeat == 0) {
                            mTimers.erase(std::remove(mTimers.begin(), mTimers.end(), instance), mTimers.end());
                        }
                    }
                }
                else {
                    std::chrono::microseconds timePoint(instance->mNext - time);
                    mCV.wait_for(lock, timePoint);
                }
            }
        }
    }
}