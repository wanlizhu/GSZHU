#pragma once

#include <thread>
#include <atomic>
#include <mutex>
#include <condition_variable>
#include <algorithm>
#include <unordered_map>
#include <string>
#include <functional>
#include <future>
#include <queue>
#include "TaskThread.h"

namespace Wanlix
{
    class ThreadPool final
    {
        class Thread;
        using ThreadMap = std::unordered_map<String, TaskThread>;
    public:
        static void Initialize(const std::vector<String>& names)
        {
            for (const auto& name : names) {
                if (!mTaskThreadMap[name].joinable()) {
                    printf("error: failed to create thread(\"%s\")\n", name.c_str());
                }
            }
        }

        static void Destroy() 
        {
            for (auto& [name, thread] : mTaskThreadMap) {
                thread.Quit();
                thread.join();
            }
        }

        template<typename _Function_, typename... ARGS>
        static auto AddTask(
            _Function_&& func,
            ARGS&& ...   args
        ) -> std::future<std::invoke_result_t<_Function_, ARGS...>>
        {
            String threadName = "*";
            auto it = std::find_if(mTaskThreadMap.begin(),
                                   mTaskThreadMap.end(),
                                   [](const ThreadMap::iterator& it) {
                                       return !it->second.IsBlocked();
                                   });
            if (it != mTaskThreadMap.end()) {
                threadName = it->first;
            }

            return AddTaskTo(threadName, 
                             std::forward<_Function_>(func), 
                             std::forward<ARGS>(args)...);
        }

        template<typename _Function_, typename... ARGS>
        static auto AddTaskTo(
            StringCRef   threadName,
            _Function_&& func,
            ARGS&& ...   args
        ) -> std::future<std::invoke_result_t<_Function_, ARGS...>>
        {
            using ReturnType = std::result_of<_Function_(ARGS...)>;
            auto task = std::make_shared<std::packaged_task<ReturnType()>>(
                    std::bind(std::forward<_Function_>(func),
                    std::forward<ARGS>(args)...)
                );
            std::future<ReturnType> future = task->get_future();

            mTaskThreadMap[threadName].PostTask([task]() { (*task)(); });

            return future;
        }

    private:
        static ThreadMap mTaskThreadMap;
    };
}