#pragma once

#include <GSZHU/Config.h>
#include <GSZHU/IWindow.h>
#include <GSZHU/IRenderer.h>
#include <GSZHU/CManagedAPI.h>
#ifdef _WIN32
#include <GSZHU/Win32/System.h>
#endif

namespace GSZHU {
    class IApplication : public CManagedAPI<IApplication> {
    public:
        virtual ~IApplication() {}
        virtual std::shared_ptr<IWindow> CreateWindow(STRING const& Name, int Width, int Height, bool VSync = true) = 0;
        virtual bool DestroyWindow(STRING const& Name) = 0;
        virtual std::shared_ptr<IWindow> FindWindowByName(STRING const& Name) const = 0;

        virtual void Run() = 0;
        virtual void Quit(int Code = 0) = 0;

    protected:
        HASH_MAP<STRING, std::shared_ptr<IWindow>> mWindowNameTable;
    };

#ifdef _WIN32
    GSZHU_API std::shared_ptr<IApplication> CreateApplication(HINSTANCE Inst);
#endif
}