#include "Window.h"
#include "AppDelegate.h"

INT main(INT argc, char** argv)
{
    Window window;
    window.Create(L"AcadDM Vulkan", 1000, 618);
    
    AppDelegate app;
    app.SetWindow(&window);
    app.LoadData();

    MSG msg = {};

    while (GetMessage(&msg, nullptr, 0, 0))
    {
        if (!TranslateMessage(&msg))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
    }

    return (INT)msg.wParam;
}