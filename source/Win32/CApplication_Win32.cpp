#include "Win32/CApplication_Win32.h"
#include <GSZHU/IOEvents.h>
#include <assert.h>

namespace GSZHU {
    IApplication* CreateApplication(void* userData) {
        auto app = CApplication_Win32::GetInstance();
        app->Initialize((HINSTANCE)userData);
        return app;
    }

    CApplication_Win32* CApplication_Win32::GetInstance() {
        static CApplication_Win32 sApp;
        return &sApp;
    }

    void CApplication_Win32::Initialize(void* userData) {
        if (mInstance == nullptr)
            mInstance = (HINSTANCE)userData;
    }

    void CApplication_Win32::Destroy() {
        mInstance = NULL;
        mWindowTable.clear();
    }

    HINSTANCE CApplication_Win32::GetHandle() const {
        assert(mInstance != NULL);
        return mInstance;
    }

    std::shared_ptr<IWindow> CApplication_Win32::CreateWindow(STRING const& name, int width, int height, bool isVSync) {
        std::shared_ptr<CWindow_Win32> window = std::make_shared<CWindow_Win32>(name, width, height, isVSync);
        if (window) {
            mWindowTable[(HWND)window->GetHandle()] = window;
        }

        return std::dynamic_pointer_cast<IWindow>(window);
    }

    void CApplication_Win32::RemoveWindow(HANDLE handle) {
        mWindowTable.erase((HWND)handle);
    }

    std::shared_ptr<IWindow> CApplication_Win32::FindWindowByName(STRING const& name) const {
        for (auto iter = mWindowTable.begin(); iter != mWindowTable.end(); iter++) {
            auto window = iter->second.lock();
            if (window && window->GetName() == name) {
                return std::dynamic_pointer_cast<IWindow>(window);
            }
        }

        return nullptr;
    }

    std::shared_ptr<IWindow> CApplication_Win32::FindWindowByHandle(HANDLE handle) const {
        auto iter = mWindowTable.find((HWND)handle);
        if (iter != mWindowTable.end()) {
            auto window = iter->second.lock();
            if (window) {
                return std::dynamic_pointer_cast<IWindow>(window);
            }
        }

        return nullptr;
    }

    int CApplication_Win32::Run() {
        MSG message = { 0 };

        while (message.message != WM_QUIT) {
            if (::PeekMessage(&message, 0, 0, 0, PM_REMOVE)) {
                ::TranslateMessage(&message);
                ::DispatchMessage(&message);
            }
        }

        return (int)(message.wParam);
    }

    void CApplication_Win32::Quit(int code) {
        ::PostQuitMessage(code);
    }
}