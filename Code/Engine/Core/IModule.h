#pragma once
#include "Common/Config.h"
#include "Common/NonCopyable.h"
#include "Common/HeapCreator.h"

namespace ZHU
{
    class ZHU_API IModule : public NonCopyable
                          , public HeapCreator
    {
    public:
        enum class EStage {
            Render,
        };

        virtual ~IModule() = default;
        virtual void Update() = 0;
    };
}