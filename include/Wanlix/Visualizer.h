#pragma once

#include "String.h"
#include "Array.h"

namespace Wanlix
{
    class WANLIX_API Visualizer
    {
        struct PrivateData;
    public:
        struct Descriptor
        {
            String     name = "Wanlix Visualizer";
            Array<int> size = { -1, -1 };
            Array<int> position = { 0, 0 };
            bool fullscreen = false;
            bool resizable = true;
            bool centered = true;
            bool borderless = false;
            bool acceptDropFiles = true;
        };

        Visualizer(const Descriptor& desc);
        virtual ~Visualizer();

        bool Initialize(const String& configFile);
        void Run();
        void Quit();

    private:
        PrivateData* mData = nullptr;
    };
}