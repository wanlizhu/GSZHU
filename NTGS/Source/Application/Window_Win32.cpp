#include "Application/Window_Win32.h"
#include "Common/Utilities.h"

namespace NTGS {
    LRESULT CALLBACK Window_Win32::WindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
        Window_Win32* win = reinterpret_cast<Window_Win32*>(::GetWindowLongPtrW(hWnd, GWLP_USERDATA));

        if (win == nullptr)
            return DefWindowProc(hWnd, uMsg, wParam, lParam);

        return DefWindowProc(hWnd, uMsg, wParam, lParam);
    }

    Window_Win32Ptr Window_Win32::Create(const std::string& name, const WindowConfig& config, void* nativeWindow) {
        Window_Win32Ptr win = std::make_shared<Window_Win32>();
        win->CreateInternal(name, config, nativeWindow);
        return win;
    }

    bool Window_Win32::CreateInternal(const std::string& name, const WindowConfig& config, void* nativeWindow) {
        mName = name;
        if (nativeWindow != nullptr) {
            mWindowHandle = static_cast<HWND>(nativeWindow);
            mDefaultProc = reinterpret_cast<WNDPROC>(::GetWindowLongPtrA(mWindowHandle, GWLP_WNDPROC));
            ::SetWindowLongPtrA(mWindowHandle, GWLP_WNDPROC, reinterpret_cast<LONG_PTR>(WindowProc));
            mExternalHandle = true;
        }
        else {
            HINSTANCE instance = GetModuleHandle(nullptr);

            WNDCLASSEXA wc;
            wc.cbSize = sizeof(wc);
            wc.style = CS_HREDRAW | CS_VREDRAW;
            wc.lpfnWndProc = WindowProc;
            wc.cbClsExtra = 0;
            wc.cbWndExtra = sizeof(this);
            wc.hInstance = instance;
            wc.hIcon = nullptr;
            wc.hCursor = ::LoadCursor(nullptr, IDC_ARROW);
            wc.hbrBackground = static_cast<HBRUSH>(::GetStockObject(BLACK_BRUSH));
            wc.lpszMenuName = nullptr;
            wc.lpszClassName = mName.c_str();
            wc.hIconSm = nullptr;
            ::RegisterClassExA(&wc);

            mWindowStyle = config.mFullScreen ? WS_POPUP : WS_OVERLAPPEDWINDOW;

            RECT rc = { 0, 0, config.mSize.x, config.mSize.y };
            ::AdjustWindowRect(&rc, mWindowStyle, false);

            mWindowHandle = ::CreateWindowA(mName.c_str(), mName.c_str(), mWindowStyle,
                                            config.mPosition.x, config.mPosition.y, config.mSize.x, config.mSize.y,
                                            0, 0, instance, nullptr);
            mDefaultProc = ::DefWindowProc;
            mExternalHandle = false;
        }
        if (mWindowHandle == nullptr) {
            DWORD error = ::GetLastError();
            printf("GetLastError() is %d\n", error);
            return false;
        }

        RECT rc;
        ::GetClientRect(mWindowHandle, &rc);
        mPosition.x = rc.left;
        mPosition.y = rc.top;
        mSize.x = rc.right - rc.left;
        mSize.y = rc.bottom - rc.top;

        ::SetWindowLongPtrA(mWindowHandle, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(this));
        
        mKeepScreenOn = config.mKeepScreenOn;
        KeepScreenOn();

        ::ShowWindow(mWindowHandle, config.mHideWindow ? SW_HIDE : SW_SHOWNORMAL);
        ::UpdateWindow(mWindowHandle);

        return true;
    }

    Window_Win32::~Window_Win32() {
        Destroy();
    }

    HWND Window_Win32::GetHandle() const {
        return mWindowHandle;
    }

    void Window_Win32::Destroy() {
        if (mWindowHandle != nullptr) {
            ::SetWindowLongPtrA(mWindowHandle, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(nullptr));
            if (!mExternalHandle) 
                ::DestroyWindow(mWindowHandle);
            mWindowHandle = nullptr;
        }
    }

    void Window_Win32::KeepScreenOn() {
        if (!mKeepScreenOn)
            return;

        ::SetThreadExecutionState(ES_CONTINUOUS | ES_SYSTEM_REQUIRED | ES_AWAYMODE_REQUIRED);
    }

    void* Window_Win32::GetID() const {
        return reinterpret_cast<void*>(mWindowHandle);
    }

    const std::string& Window_Win32::GetName() const {
        return mName;
    }

    const Vec2i& Window_Win32::GetPosition() const {
        return mPosition;
    }
    
    const Vec2i& Window_Win32::GetSize() const {
        return mSize;
    }
}