#pragma once

#include <string>
#include <vector>
#include <unordered_map>
#include "Application/IApplication.h"
#include "Application/IWindow.h"
#include "Core/BasicTypes.h"

namespace Wanli
{
    class Launch final
    {
    public:
        static int Main(int argc, char** argv);

    private:
        static void SetCommandLineArgs(int argc, char** argv);
        static void Establish();
        static void Configurate();

    private:
        StringArrayMap mCommandLineArgs;
    };
}