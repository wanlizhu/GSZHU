#include <iostream>
#include <GSZHU/IApplication.h>
#include <Windows.h>
#ifdef CreateWindow
#undef CreateWindow
#endif

using namespace GSZHU;

int CALLBACK main(HINSTANCE hInstance, HINSTANCE hPrevInstance, PWSTR lpCmdLine, int nCmdShow)
{
    IApplication* app = CreateApplication(hInstance);
    std::shared_ptr<IWindow> window = app->CreateWindow("GSZHU (DX12)", 1000, 618);
    window->Show();

    
    return app->Run();
}