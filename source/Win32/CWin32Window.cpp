#include <GSZHU/Win32/CWin32Window.h>
#include <GSZHU/Win32/CWin32Application.h>

namespace GSZHU {
    CWin32Window::CWin32Window(STRING const& Name, int Width, int Height, bool VSync) 
        : IWindow(Name, INT4{ 0, 0, Width, Height }, VSync) {
        mWindow = System::CreateWindow(Name.c_str(), Width, Height, GLOBAL_WindowProc, VSync, GLOBAL_AppInstance);
        if (mWindow) {
            ::SetWindowLongPtrA(mWindow, GWLP_USERDATA, (LONG_PTR)this);
        }
    }

    CWin32Window::~CWin32Window() {
        Destroy();
    }

    void* CWin32Window::GetNativeHandle() const {
        return mWindow;
    }

    STRING const& CWin32Window::GetName() const {
        return mName;
    }

    void CWin32Window::Initialize() {
    
    }


    void CWin32Window::Destroy() {
        if (auto Handler = mUserInputHandler.lock()) {
            Handler->OnWindowDestroy();
        }

        if (mWindow) {
            ::DestroyWindow(mWindow);
            mWindow = NULL;
        }
    }

    INT2 CWin32Window::GetSize() const {
        return INT2{ mGeometry[2], mGeometry[3] };
    }

    INT2 CWin32Window::GetPosition() const {
        return INT2{ mGeometry[0], mGeometry[1] };
    }

    bool CWin32Window::IsValid() const {
        return mWindow != NULL;
    }

    bool CWin32Window::IsVSync() const {
        return mVSync;
    }

    void CWin32Window::SetVSync(bool Enabled) {
        mVSync = Enabled;
    }

    bool CWin32Window::IsFullScreen() const {
        return mFullScreen;
    }

    void CWin32Window::SetFullScreen(bool Enabled) {
        if (mFullScreen == Enabled)
            return;

        mFullScreen = Enabled;

        if (mFullScreen) {
            // Store the current window dimensions 
            ::GetWindowRect(mWindow, &mNormalRect);

            // Set the window style to a borderless window
            UINT Style = WS_OVERLAPPEDWINDOW & ~(WS_CAPTION | WS_SYSMENU | WS_THICKFRAME | WS_MINIMIZEBOX | WS_MAXIMIZEBOX);
            ::SetWindowLongA(mWindow, GWL_STYLE, Style);

            // Query the name of the nearest display device for the window
            HMONITOR hMonitor = ::MonitorFromWindow(mWindow, MONITOR_DEFAULTTONEAREST);
            MONITORINFOEX monitorInfo = {};
            monitorInfo.cbSize = sizeof(MONITORINFOEX);
            ::GetMonitorInfo(hMonitor, &monitorInfo);

            ::SetWindowPos(mWindow, HWND_TOPMOST,
                           monitorInfo.rcMonitor.left,
                           monitorInfo.rcMonitor.top,
                           monitorInfo.rcMonitor.right - monitorInfo.rcMonitor.left,
                           monitorInfo.rcMonitor.bottom - monitorInfo.rcMonitor.top,
                           SWP_FRAMECHANGED | SWP_NOACTIVATE);
            ::ShowWindow(mWindow, SW_MAXIMIZE);
        }
        else {
            // Restore all the window decorators.
            ::SetWindowLong(mWindow, GWL_STYLE, WS_OVERLAPPEDWINDOW);

            ::SetWindowPos(mWindow, HWND_NOTOPMOST,
                mNormalRect.left,
                mNormalRect.top,
                mNormalRect.right - mNormalRect.left,
                mNormalRect.bottom - mNormalRect.top,
                SWP_FRAMECHANGED | SWP_NOACTIVATE);

            ::ShowWindow(mWindow, SW_NORMAL);
        }
    }

    void CWin32Window::Show() {
        ::ShowWindow(mWindow, SW_SHOW);
    }

    void CWin32Window::Hide() {
        ::ShowWindow(mWindow, SW_HIDE);
    }

    void CWin32Window::SetUserInputHandler(std::weak_ptr<IUserInput> Handler) {
        mUserInputHandler = Handler;
    }

    void CWin32Window::OnWin32Message(HWND Window, UINT Message, WPARAM wParam, LPARAM lParam) {
    
    }
}