#include "Wanlix/Core3D/Platform/Win32/Win32Window.h"
#include "Wanlix/Core3D/Platform/MapKey.h"
#include "Win32WindowClass.h"
#include <windowsx.h>

#ifndef HID_USAGE_PAGE_GENERIC
#   define HID_USAGE_PAGE_GENERIC   ((USHORT)0x01)
#endif
#ifndef HID_USAGE_GENERIC_MOUSE
#   define HID_USAGE_GENERIC_MOUSE  ((USHORT)0x02)
#endif

namespace Wanlix
{
    static Win32Window* GetWindowFromUserData(HWND wnd)
    {
        return reinterpret_cast<Win32Window*>(GetWindowLongPtr(wnd, GWLP_USERDATA));
    }

    static void PostKeyEvent(
        IWindow& window, 
        Key keyCode,
        bool isDown
    )
    {
        if (isDown)
            window.OnKeyDown(window, keyCode);
        else
            window.OnKeyUp(window, keyCode);
    }

    static void PostKeyEvent(
        HWND wnd,
        WPARAM wParam,
        LPARAM lParam,
        bool isDown,
        bool isSysKey
    )
    {
        // Get window object from window handle 
        if (auto window = GetWindowFromUserData(wnd))
        {
            // Extract key code 
            auto keyCodeSys = static_cast<uint32_t>(wParam);
            auto keyCodeOEM = static_cast<uint32_t>(lParam & (0xff << 16)) >> 16;
            bool isExtendedKey = ((lParam & (1 << 24)) != 0);

            // Get key code mapping first 
            auto keyCode = MapKey(static_cast<uint32_t>(keyCodeSys));

            // Check for extended keys 
            switch (keyCode)
            {
            case Key::Shift:
            {
                if (keyCodeOEM == 0x36)
                    PostKeyEvent(*window, Key::RShift, isDown);
                else if (keyCodeOEM == 0x2a)
                    PostKeyEvent(*window, Key::LShift, isDown);
            }
            break;

            case Key::Control:
            {
                if (isExtendedKey)
                    PostKeyEvent(*window, Key::RControl, isDown);
                else
                    PostKeyEvent(*window, Key::LControl, isDown);
            }
            break;
            }

            // Post base key event 
            Wanlix::PostKeyEvent(*window, keyCode, isDown);
        }
    }

    static int g_mouseCaptureCounter = 0;

    static void ReleaseMouseCapture()
    {
        if (g_mouseCaptureCounter > 0)
        {
            g_mouseCaptureCounter = 0;
            ReleaseCapture();
        }
    }

    static void CaptureMouseButton(
        HWND wnd,
        Key keyCode,
        bool doubleClick = false
    )
    {
        // Get window object from window handle 
        if (auto window = GetWindowFromUserData(wnd))
        {
            // Post key events and capture mouse 
            window->OnKeyDown(*window, keyCode);

            if (doubleClick)
                window->OnDoubleClick(*window, keyCode);

            if (++g_mouseCaptureCounter == 1)
                SetCapture(wnd);
        }
    }

    static void ReleaseMouseButton(
        HWND wnd,
        Key keyCode
    )
    {
        // Get window object from window handle 
        if (auto window = GetWindowFromUserData(wnd))
        {
            // Post key event and release mouse capture 
            window->OnKeyUp(*window, keyCode);

            if (--g_mouseCaptureCounter == 0)
                ReleaseCapture();

            if (g_mouseCaptureCounter < 0)
            {
                //TODO: this condition should never be true!!!
                g_mouseCaptureCounter = 0;
            }
        }
    }

    static void PostLocalMouseMotion(
        HWND wnd,
        LPARAM lParam
    )
    {
        // Get window object from window handle 
        if (auto window = GetWindowFromUserData(wnd))
        {
            // Extract mouse position from event parameter 
            int x = GET_X_LPARAM(lParam);
            int y = GET_Y_LPARAM(lParam);

            // Post local mouse motion event 
            window->OnMouseMotion(*window, Offset(x, y));
        }
    }

    LRESULT CALLBACK Win32WindowClass::WindowProc(
        HWND wnd,
        UINT msg,
        WPARAM wParam,
        LPARAM lParam
    )
    {
        switch (msg)
        {
            // --- Common window events --- 

        case WM_CREATE:
        {
            // Register raw input device to capture high-resolution mouse motion events 
            RAWINPUTDEVICE device;

            device.usUsagePage = HID_USAGE_PAGE_GENERIC;
            device.usUsage = HID_USAGE_GENERIC_MOUSE;
            device.dwFlags = RIDEV_INPUTSINK;
            device.hwndTarget = wnd;

            RegisterRawInputDevices(&device, 1, sizeof(device));
        }
        break;

        case WM_SIZE:
        {
            // Post resize event to window 
            if (auto window = GetWindowFromUserData(wnd))
            {
                WORD width = LOWORD(lParam);
                WORD height = HIWORD(lParam);
                window->OnResize(*window, 
                                 Extent(
                                     static_cast<uint32_t>(width),
                                     static_cast<uint32_t>(height) 
                                 ));
            }
        }
        break;

        case WM_CLOSE:
        {
            // Post close event to window 
            if (auto window = GetWindowFromUserData(wnd)) {
                window->Quit();
            }
               
        }
        break;

        case WM_SYSCOMMAND:
        {
            #if 0
            switch (wParam & 0xfff0)
            {
            case SC_SCREENSAVE:
            case SC_MONITORPOWER:
            {
                if (auto window = GetWindowFromUserData(wnd))
                {
                    if (window->GetDesc().preventForPowerSafe)
                    {
                        // Prevent for a powersave mode of monitor or the screensaver 
                        return 0;
                    }
                }
            }
            break;
            }
            #endif
        }
        break;

        case WM_SETFOCUS:
        {
            if (auto window = GetWindowFromUserData(wnd)) {
                window->OnGetFocus(*window);
            }
        }
        break;

        case WM_KILLFOCUS:
        {
            ReleaseMouseCapture();
            if (auto window = GetWindowFromUserData(wnd)) {
                window->OnLostFocus(*window);
            }
        }
        break;

        // --- Keyboard events --- 

        case WM_KEYDOWN:
        {
            PostKeyEvent(wnd, wParam, lParam, true, false);
        }
        return 0;

        case WM_KEYUP:
        {
            PostKeyEvent(wnd, wParam, lParam, false, false);
        }
        return 0;

        case WM_SYSKEYDOWN:
        {
            PostKeyEvent(wnd, wParam, lParam, true, true);
        }
        return 0;

        case WM_SYSKEYUP:
        {
            PostKeyEvent(wnd, wParam, lParam, false, true);
        }
        return 0;

        case WM_CHAR:
        {
            if (auto window = GetWindowFromUserData(wnd))
                window->OnChar(*window, static_cast<wchar_t>(wParam));
        }
        return 0;

        // --- Left mouse button events --- 

        case WM_LBUTTONDOWN:
        {
            CaptureMouseButton(wnd, Key::LButton);
        }
        return 0;

        case WM_LBUTTONUP:
        {
            ReleaseMouseButton(wnd, Key::LButton);
        }
        return 0;

        case WM_LBUTTONDBLCLK:
        {
            CaptureMouseButton(wnd, Key::LButton, true);
        }
        return 0;

        // --- Right mouse button events --- 

        case WM_RBUTTONDOWN:
        {
            CaptureMouseButton(wnd, Key::RButton);
        }
        return 0;

        case WM_RBUTTONUP:
        {
            ReleaseMouseButton(wnd, Key::RButton);
        }
        return 0;

        case WM_RBUTTONDBLCLK:
        {
            CaptureMouseButton(wnd, Key::RButton, true);
        }
        return 0;

        // --- Middle mouse button events --- 

        case WM_MBUTTONDOWN:
        {
            CaptureMouseButton(wnd, Key::MButton);
        }
        return 0;

        case WM_MBUTTONUP:
        {
            ReleaseMouseButton(wnd, Key::MButton);
        }
        return 0;

        case WM_MBUTTONDBLCLK:
        {
            CaptureMouseButton(wnd, Key::MButton, true);
        }
        return 0;

        // --- Mouse motion events --- 

        case WM_MOUSEWHEEL:
        {
            if (auto window = GetWindowFromUserData(wnd))
                window->OnWheelMotion(*window, GET_WHEEL_DELTA_WPARAM(wParam) / WHEEL_DELTA);
        }
        return 0;

        case WM_MOUSEMOVE:
        {
            PostLocalMouseMotion(wnd, lParam);
        }
        return 0;

        case WM_INPUT:
        {
        }
        return 0;

        // --- Misc events --- 

        case WM_ERASEBKGND:
        {
            // Do not erase background to avoid flickering when user resizes the window 
            if (auto window = GetWindowFromUserData(wnd))
            {
                return 0;
            }
        }
        break;

        case WM_ENTERSIZEMOVE:
        {
            if (auto window = GetWindowFromUserData(wnd))
            {
                auto timerId = window->GetMoveAndResizeTimerId();
                ::SetTimer(wnd, timerId, USER_TIMER_MINIMUM, NULL);
            }
        }
        break;

        case WM_EXITSIZEMOVE:
        {
            if (auto window = GetWindowFromUserData(wnd))
            {
                auto timerId = window->GetMoveAndResizeTimerId();
                ::KillTimer(wnd, timerId);
            }
        }
        break;

        case WM_TIMER:
        {
            if (auto window = GetWindowFromUserData(wnd))
            {
                auto timerId = static_cast<uint32_t>(wParam);
                if (timerId == window->GetMoveAndResizeTimerId()) {
                    window->OnDraw(*window);
                }
            }
        };
        break;
        }

        return ::DefWindowProcW(wnd, msg, wParam, lParam);
    }
}
