#include <GSZHU/Win32/System.h>

namespace GSZHU::System {
    constexpr char GLOBAL_WINDOW_CLASS_NAME[] = "DX12RenderWindowClass";
    static bool GLOBAL_CLASS_REGISTERED = false;

    HWND CreateWindow(const char* Name, LONG Width, LONG Height, WNDPROC WinProc, bool VSync, HINSTANCE Instance) {
        // Unregister window class
        if (GLOBAL_CLASS_REGISTERED) {
            UnregisterClassA(GLOBAL_WINDOW_CLASS_NAME, Instance);
        }

        // Register window class
        WNDCLASSEXA WindowClass = { 0 };

        WindowClass.cbSize = sizeof(WNDCLASSEX);
        WindowClass.style = CS_HREDRAW | CS_VREDRAW;
        WindowClass.lpfnWndProc = WinProc;
        WindowClass.hInstance = Instance;
        WindowClass.hCursor = LoadCursor(nullptr, IDC_ARROW);
        WindowClass.hIcon = NULL;
        WindowClass.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
        WindowClass.lpszMenuName = nullptr;
        WindowClass.lpszClassName = GLOBAL_WINDOW_CLASS_NAME;
        WindowClass.hIconSm = NULL;

        if (!RegisterClassExA(&WindowClass)) {
            MessageBoxA(NULL, "Unable to register the window class.", "Error", MB_OK | MB_ICONERROR);
            return nullptr;
        }

        //Create window
        RECT WindowRect = { 0, 0, Width, Height };
        AdjustWindowRect(&WindowRect, WS_OVERLAPPEDWINDOW, FALSE);
        HWND Window = CreateWindowA(GLOBAL_WINDOW_CLASS_NAME, Name, WS_OVERLAPPEDWINDOW,
                                    CW_USEDEFAULT, CW_USEDEFAULT,
                                    WindowRect.right - WindowRect.left, WindowRect.bottom - WindowRect.top,
                                    nullptr, nullptr, Instance, nullptr);
        if (!Window) {
            MessageBoxA(NULL, "Could not create the render window.", "Error", MB_OK | MB_ICONERROR);
            return nullptr;
        }

        return Window;
    }
}