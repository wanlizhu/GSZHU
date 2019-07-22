#include <iostream>
#include "Engine/GUI/Window.h"

using namespace ZHU;

int main()
{
    Window::Initialize();
    Window window("Graphics Engine", SCREEN_HALF, SCREEN_CENTER);
    window.MainLoop();

    return 0;
}
