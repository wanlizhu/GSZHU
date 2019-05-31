#include "Win32/System_Win32.h"
#include "resource/resource.h"

namespace GSZHU::System {
    constexpr char GLOBAL_WINDOW_CLASS_NAME[] = "DX12RenderWindowClass";
    static bool GLOBAL_CLASS_REGISTERED = false;

    HWND CreateWindow(const char* name, LONG width, LONG height, WNDPROC winProc, bool isVSync, HINSTANCE instance) {
        // Unregister window class
        if (GLOBAL_CLASS_REGISTERED) {
            ::UnregisterClass(GLOBAL_WINDOW_CLASS_NAME, instance);
        }

        // Register window class
        WNDCLASSEXA windowClass = { 0 };

        windowClass.cbSize = sizeof(WNDCLASSEX);
        windowClass.style = CS_HREDRAW | CS_VREDRAW;
        windowClass.lpfnWndProc = winProc;
        windowClass.hInstance = instance;
        windowClass.hCursor = ::LoadCursor(NULL, IDC_ARROW);
        windowClass.hIcon = NULL;
        windowClass.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
        windowClass.lpszMenuName = nullptr;
        windowClass.lpszClassName = GLOBAL_WINDOW_CLASS_NAME;
        windowClass.hIconSm = NULL;

        if (!::RegisterClassEx(&windowClass)) {
            ::MessageBox(NULL, "Unable to register the window class.", "Error", MB_OK | MB_ICONERROR);
            return nullptr;
        }

        //Create window
        RECT windowRect = { 0, 0, width, height };
        ::AdjustWindowRect(&windowRect, WS_OVERLAPPEDWINDOW, FALSE);
        HWND window = ::CreateWindowEx(0, GLOBAL_WINDOW_CLASS_NAME, name, WS_OVERLAPPEDWINDOW,
                                       CW_USEDEFAULT, CW_USEDEFAULT,
                                       windowRect.right - windowRect.left, windowRect.bottom - windowRect.top,
                                       nullptr, nullptr, instance, nullptr);
        if (!window) {
            ::MessageBox(NULL, "Could not create the render window.", "Error", MB_OK | MB_ICONERROR);
            return nullptr;
        }

        return window;
    }
}