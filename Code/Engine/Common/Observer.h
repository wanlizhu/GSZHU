#pragma once
#include "Config.h"
#include <memory>

namespace ZHU
{
    class Observer
    {
    public:
        friend class ObserverVisitor;
        Observer()
            : mIsValid(std::make_shared<bool>())
        {}

    private:
        std::shared_ptr<bool> mIsValid;
    };
}