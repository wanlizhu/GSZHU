#include "Win32/CWindow_Win32.h"
#include "Win32/CApplication_Win32.h"
#include "Win32/WinProc_Win32.h"
#include <assert.h>

namespace GSZHU {
    CWindow_Win32::CWindow_Win32(STRING const& name, int width, int height, bool isVSync)
        : mName(name)
        , mVSync(isVSync)
        , mFullScreen(width <= 0 || height <= 0) {
        mHandle = System::CreateWindow(name.c_str(), width, height, WinProc_Win32, isVSync, 
                                       CApplication_Win32::GetInstance()->GetHandle());
        assert(mHandle != NULL);
    }

    CWindow_Win32::~CWindow_Win32() {
        Destroy();
    }

    HANDLE CWindow_Win32::GetHandle() const {
        return mHandle;
    }

    STRING const& CWindow_Win32::GetName() const {
        return mName;
    }

    void CWindow_Win32::Initialize() {
    
    }


    void CWindow_Win32::Destroy() {
        if (auto handler = mRenderer.lock()) {
            handler->OnDestroy();
        }

        if (mHandle) {
            ::DestroyWindow(mHandle);
            mHandle = NULL;
        }
    }

    INT2 CWindow_Win32::GetSize() const {
        RECT rect;
        assert(mHandle != NULL);
        ::GetWindowRect(mHandle, &rect);
        return INT2{ rect.right - rect.left, rect.bottom - rect.top };
    }

    INT2 CWindow_Win32::GetPosition() const {
        RECT rect;
        assert(mHandle != NULL);
        ::GetWindowRect(mHandle, &rect);
        return INT2{ rect.left, rect.top };
    }

    bool CWindow_Win32::IsValid() const {
        return mHandle != NULL;
    }

    bool CWindow_Win32::IsVSync() const {
        return mVSync;
    }

    void CWindow_Win32::SetVSync(bool enabled) {
        mVSync = enabled;
    }

    bool CWindow_Win32::IsFullScreen() const {
        return mFullScreen;
    }

    void CWindow_Win32::SetFullScreen(bool enabled) {
        if (mFullScreen == enabled)
            return;

        mFullScreen = enabled;

        if (mFullScreen) {
            // Store the current window dimensions 
            ::GetWindowRect(mHandle, &mNormalRect);

            // Set the window style to a borderless window
            UINT style = WS_OVERLAPPEDWINDOW & ~(WS_CAPTION | WS_SYSMENU | WS_THICKFRAME | WS_MINIMIZEBOX | WS_MAXIMIZEBOX);
            ::SetWindowLongA(mHandle, GWL_STYLE, style);

            // Query the name of the nearest display device for the window
            HMONITOR hMonitor = ::MonitorFromWindow(mHandle, MONITOR_DEFAULTTONEAREST);
            MONITORINFOEX monitorInfo = {};
            monitorInfo.cbSize = sizeof(MONITORINFOEX);
            ::GetMonitorInfo(hMonitor, &monitorInfo);

            ::SetWindowPos(mHandle, HWND_TOPMOST,
                           monitorInfo.rcMonitor.left,
                           monitorInfo.rcMonitor.top,
                           monitorInfo.rcMonitor.right - monitorInfo.rcMonitor.left,
                           monitorInfo.rcMonitor.bottom - monitorInfo.rcMonitor.top,
                           SWP_FRAMECHANGED | SWP_NOACTIVATE);
            ::ShowWindow(mHandle, SW_MAXIMIZE);
        }
        else {
            // Restore all the window decorators.
            ::SetWindowLong(mHandle, GWL_STYLE, WS_OVERLAPPEDWINDOW);

            ::SetWindowPos(mHandle, HWND_NOTOPMOST,
                mNormalRect.left,
                mNormalRect.top,
                mNormalRect.right - mNormalRect.left,
                mNormalRect.bottom - mNormalRect.top,
                SWP_FRAMECHANGED | SWP_NOACTIVATE);

            ::ShowWindow(mHandle, SW_NORMAL);
        }
    }

    void CWindow_Win32::Show() {
        ::ShowWindow(mHandle, SW_SHOW);
    }

    void CWindow_Win32::Hide() {
        ::ShowWindow(mHandle, SW_HIDE);
    }

    void CWindow_Win32::OnEvent(IEvent const& event) {
        
    }

    void CWindow_Win32::PostEvent(IEvent const& event) {

    }

    void CWindow_Win32::SetUserInputHandler(std::weak_ptr<IUserInput> handler) {
        mUserInputHandler = handler;
    }

    void CWindow_Win32::SetRenderer(std::weak_ptr<IRenderer> renderer) {
        mRenderer = renderer;
    }

    INT2 const& CWindow_Win32::GetLastMousePos() const {
        return mLastMousePos;
    }

    CWindowClock const& CWindow_Win32::GetWindowClock() const {
        return mWindowClock;
    }
}