#pragma once

#include "Core/Common.h"

namespace Wanli
{
    class WANLI_API IConfig
    {
    public:
        IConfig() = default;
        virtual ~IConfig() = default;

        inline bool IsCompleted() const 
        {
            return mCompleted;
        }

        inline bool IsDependencyResolved() const
        {
            return std::all_of(mDependencies.begin(), mDependencies.end(),
                [&](IConfig* config) {
                    return config->IsCompleted();
                });
        }

        inline void SetOnCompleted(const std::function<void()>& func)
        {
            mCompletedFunc = func;
        }

        inline void OnCompleted() 
        {
            mCompleted = true;
            if (mCompletedFunc)
            {
                mCompletedFunc();
            }
        }

        inline void AddDependency(IConfig* config)
        {
            mDependencies.push_back(config);
        }

    private:
        std::atomic_bool mCompleted = false;
        std::function<void()> mCompletedFunc;
        std::vector<IConfig*> mDependencies;
    };
}