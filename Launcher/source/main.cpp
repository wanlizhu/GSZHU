#include <iostream>
#include "Engine/GUI/Window.h"

using namespace ZHU;

int main()
{
    Window::CreationInfo info;
    info.title = "Graphics Engine";
    info.size = Window::DefaultSize();
    info.pos = Window::DefaultPos();
    Window window(info);
    window.MainLoop();

    return 0;
}
