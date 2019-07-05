#pragma once
#include "Config.h"
#include <memory>

namespace ZHU
{
    class ZHU_API HeapCreator : public std::enable_shared_from_this<HeapCreator>
    {
    protected:
        // Use std::make_shared<>() to create, 
        // otherwise throw std::runtime_error
        HeapCreator(); 
        virtual ~HeapCreator() = default;
    };
}