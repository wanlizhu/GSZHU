#pragma once

#include "Common.h"
#include "NonCopyable.h"

namespace Wanlix
{
    class Engine final : public NonCopyable
    {
    public:
        static Ptr Create(Surface::Ptr surface);
        ~Engine();

        void Tick();
        
    private:
        Engine();
        bool LoadConfigFile();
        bool InitializeModules();

    private:
        
    };
}