#pragma once
#include <ZHUENGINE/Core/Common/Timer.h>
#include <ZHUENGINE/Core/IModule.h>
#include <ZHUENGINE/Core/Engine.h>

namespace ZHU
{
    class ZHU_API Timers final : public IModule
    {
    public:
        inline static Timers* Get() { return Engine::Get()->GetModule<Timers>(); }

        Timers();
        virtual ~Timers();

        virtual void Update() override;

        template<typename... ARGS>
        Timer* Once(const Time& delay, std::function<void()>&& function, ARGS... args)
        {
            std::unique_lock<std::mutex> lock(mMutex);

            auto instance = std::make_unique<Timer>(delay, 1);

            if constexpr (sizeof...(ARGS) == 0) {
                instance->mOnTick.Add(std::move(function));
            }
            else {
                instance->mOnTick.Add(std::move(std::bind(function, std::forward<ARGS>(args)...)));
            }
            mTimers.emplace_back(std::move(instance));

            mCV.notify_all();
            return instance.get();
        }

        template<typename... ARGS>
        Timer* Every(const Time& interval, std::function<void()>&& function, ARGS... args)
        {
            std::unique_lock<std::mutex> lock(mMutex);
            auto instance = std::make_unique<Timer>(interval, std::nullopt);

            if constexpr (sizeof...(ARGS) == 0) {
                instance->mOnTick.Add(std::move(function));
            }
            else {
                instance->mOnTick.Add(std::move(std::bind(function, std::forward<ARGS>(args)...)));
            }
            mTimers.emplace_back(std::move(instance));

            mCV.notify_all();
            return instance.get();
        }

        template<typename... ARGS>
        Timer* Repeat(const Time& interval, const uint32_t& repeat, std::function<void()>&& function, ARGS... args)
        {
            std::unique_lock<std::mutex> lock(mMutex);
            auto instance = std::make_unique<Timer>(interval, repeat);

            if constexpr (sizeof...(ARGS) == 0) {
                instance->mOnTick.Add(std::move(function));
            }
            else {
                instance->mOnTick.Add(std::move(std::bind(function, std::forward<ARGS>(args)...)));
            }
            mTimers.emplace_back(std::move(instance));

            mCV.notify_all();
            return instance.get();
        }

    private:
        void Run();

    private:
        std::vector<std::unique_ptr<Timer>> mTimers;
        std::atomic<bool> mQuit = false;
        std::thread mWorker;
        std::mutex mMutex;
        std::condition_variable mCV;
    };
}