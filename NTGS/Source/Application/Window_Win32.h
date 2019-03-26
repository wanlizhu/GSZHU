#pragma once

#include "Application/IWindow.h"
#include <windows.h>

namespace NTGS {
    class Window_Win32;
    typedef std::shared_ptr<Window_Win32> Window_Win32Ptr;

    class Window_Win32 : public IWindow {
        friend class Application_Win32;
    public:
        virtual ~Window_Win32();
        virtual void Destroy() override;
        virtual void KeepScreenOn() override;
        virtual void* GetID() const override;
        virtual const std::string& GetName() const override;
        virtual const Vec2i& GetPosition() const override;
        virtual const Vec2i& GetSize() const override;
        HWND GetHandle() const;

    private:
        static Window_Win32Ptr Create(const std::string& name, const WindowConfig& config, void* nativeWindow);
        bool CreateInternal(const std::string& name, const WindowConfig& config, void* nativeWindow);
        static LRESULT CALLBACK WindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

    private:
        HWND mWindowHandle;
        DWORD mWindowStyle = 0;
        WNDPROC mDefaultProc;
        bool mExternalHandle = false;

        std::string mName;
        Vec2i mPosition;
        Vec2i mSize;
        bool mKeepScreenOn = false;
    };
}