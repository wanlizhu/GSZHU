#pragma once

#include <GSZHU/IApplication.h>
#include <GSZHU/IWindow.h>
#include <GSZHU/Win32/CWin32Window.h>

namespace GSZHU {
    static HINSTANCE GLOBAL_AppInstance;
    LRESULT CALLBACK GLOBAL_WindowProc(HWND Window, UINT Message, WPARAM wParam, LPARAM lParam);

    class CWin32Application : public IApplication {
        friend LRESULT CALLBACK GLOBAL_WindowProc(HWND Window, UINT Message, WPARAM wParam, LPARAM lParam);
    public:
        CWin32Application(HINSTANCE Inst);
        virtual ~CWin32Application();

        virtual std::shared_ptr<IWindow> CreateWindow(STRING const& Name, int Width, int Height, bool VSync = true) override;
        virtual bool DestroyWindow(STRING const& Name) override;
        virtual std::shared_ptr<IWindow> FindWindowByName(STRING const& Name) const override;

        virtual void Run() override;
        virtual void Quit(int Code = 0) override;
        
    private:
        HINSTANCE mInstance = NULL;
    };
}