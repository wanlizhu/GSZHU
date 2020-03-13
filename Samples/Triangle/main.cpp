#include "MainApp.h"

int main(int argc, char** argv)
{
    using namespace Wanli;

    EngineConfig::Get()->SetArgList(ArgList(argc, argv));
    EngineConfig::Get()->SetEnableRenderThread(false);

    WindowConfig::Get()->SetTitle("Triangle");
    WindowConfig::Get()->SetSize({ 0, 0 });
    WindowConfig::Get()->SetAttribs(WindowAttrbutes());
    WindowConfig::Get()->AddDependency(EngineConfig::Get());
    WindowConfig::Get()->SetOnCompleted([&]() {
            GraphicsConfig::Get()->SetNativeWindow(Window::Get()->GetHwnd());
        });

    GraphicsConfig::Get()->SetAsyncDraw(true);
    GraphicsConfig::Get()->SetBackbufferCount(2);
    GraphicsConfig::Get()->SetBackgroundColor(Color_AutoCAD);
    GraphicsConfig::Get()->SetEnableDebug(true);
    GraphicsConfig::Get()->AddDependency(WindowConfig::Get());
    InputConfig::Get()->AddDependency(WindowConfig::Get());
    
    Engine::Get()->SetApp(std::make_unique<MainApp>());
    return Engine::Get()->Run();
}