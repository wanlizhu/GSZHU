#include <GSZHU/Win32/CWin32Application.h>

namespace GSZHU {
    HINSTANCE GLOBAL_AppInstance = NULL;

    LRESULT CALLBACK GLOBAL_WindowProc(HWND Window, UINT Message, WPARAM wParam, LPARAM lParam) {

    }

    CWin32Application::CWin32Application(HINSTANCE Inst)
        : mInstance(Inst) {
        GLOBAL_AppInstance = Inst;
    }

    CWin32Application::~CWin32Application() {
        Quit(0);
    }

    std::shared_ptr<IWindow> CWin32Application::CreateWindow(STRING const& Name, int Width, int Height, bool VSync) {
        std::shared_ptr<CWin32Window> Win = std::make_shared<CWin32Window>(Name, Width, Height, VSync);
        if (Win) {
            mWindowNameTable[Name] = std::dynamic_pointer_cast<IWindow>(Win);
            return mWindowNameTable[Name];
        }

        return std::dynamic_pointer_cast<IWindow>(Win);
    }

    bool CWin32Application::DestroyWindow(STRING const& Name) {
        auto Iter = mWindowNameTable.find(Name);
        if (Iter != mWindowNameTable.end()) {
            auto Win = Iter->second;
            std::weak_ptr<IWindow> Weak(Win);

            mWindowNameTable.erase(Iter);
            Win.reset();

            return Weak.expired(); // Check if there shared object is really destroyed now
        }

        return false;
    }

    std::shared_ptr<IWindow> CWin32Application::FindWindowByName(STRING const& Name) const {
        auto Iter = mWindowNameTable.find(Name);
        if (Iter != mWindowNameTable.end()) {
            return Iter->second;
        }

        return nullptr;
    }

    void CWin32Application::Run() {

    }

    void CWin32Application::Quit(int Code) {
        PostQuitMessage(Code);
    }

    std::shared_ptr<IApplication> CreateApplication(HINSTANCE Inst) {
        auto App = std::make_shared<CWin32Application>(Inst);
        return std::dynamic_pointer_cast<IApplication>(App);
    }
}