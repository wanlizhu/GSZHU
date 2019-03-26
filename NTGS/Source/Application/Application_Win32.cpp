#include "Application/Application_Win32.h"

namespace NTGS {
    Application_Win32Ptr Application_Win32::Create() {
        return std::make_shared<Application_Win32>();
    }

    Application_Win32::~Application_Win32() {
        Finalize();
    }

    void Application_Win32::Initialize() {
    
    }

    void Application_Win32::Finalize() {
        for (auto& win : mWindowList) {
            win->Destroy();
        }
        mWindowList.clear();
    }

    void Application_Win32::Run() {
        MSG msg;

        ::PeekMessage(&msg, nullptr, 0, 0, PM_NOREMOVE);

        while (msg.message != WM_QUIT) {
            if (::GetMessage(&msg, nullptr, 0, 0) != 0) {
                ::TranslateMessage(&msg);
                ::DispatchMessage(&msg);
            }
        }
    }

    void Application_Win32::Quit() {
        Finalize();
    }

    IWindowPtr Application_Win32::CreateNamedWindow(const std::string& name, const WindowConfig& config, void* nativeWindow) {
        Window_Win32Ptr win = Window_Win32::Create(name, config, nativeWindow);
        mWindowList.push_back(win);
        return win;
    }

    IWindowPtr Application_Win32::GetWindow(const std::string& name) const {
        Window_Win32Ptr win;
        for (size_t i = 0; i < mWindowList.size(); i++) {
            if (mWindowList[i]->GetName() == name) {
                win = mWindowList[i];
                break;
            }
        }
        return win;
    }
}