#include "Wanlix/ThreadPool.h"

namespace UT
{
    std::vector<std::thread> ThreadPool::smThreads;
    std::queue<std::function<void()>> ThreadPool::smTaskQueue;
    std::unordered_map<std::string, std::thread::id> ThreadPool::smThreadNameMap;
    std::queue<std::string> ThreadPool::smNameQueue;

    std::mutex ThreadPool::smMutex;
    std::condition_variable ThreadPool::smCondition;
    std::atomic_bool ThreadPool::smStopFlag;


    void ThreadPool::Initialize(size_t count) {
        std::vector<std::string> names(count);
        for (int i = 0; i < count; i++)
            names.push_back(std::to_string(i));
        Initialize(names);
    }

    void ThreadPool::Initialize(const std::vector<std::string>& names)
    {
        Destroy();
        smStopFlag.store(false);

        for (int i = 0; i < (int)names.size(); i++) {
            smThreads.emplace_back([]() {
                while (true) {
                    std::function<void()> task;

                    { // auto-unlock block
                        std::unique_lock<std::mutex> lock(smMutex);
                        smCondition.wait(lock, []() {
                            bool isMyName = (smNameQueue.front() == "*" || 
                                             smThreadNameMap[smNameQueue.front()] == std::this_thread::get_id());
                            
                            return smStopFlag.load() || (!smTaskQueue.empty() && isMyName);
                        });

                        if (smStopFlag.load() && smTaskQueue.empty()) {
                            return;
                        }

                        task = std::move(smTaskQueue.front());
                        smTaskQueue.pop();
                        smNameQueue.pop();
                    }

                    task();
                } // while (true)
                                   });
            smThreadNameMap[names[i]] = smThreads.back().get_id();
        }
    }

    void ThreadPool::Destroy()
    {
        smStopFlag.store(true);
        smCondition.notify_all();
        for (auto& thread : smThreads) {
            if (thread.joinable())
                thread.join();
        }
        smThreads.clear();
        smThreadNameMap.clear();
    }
}