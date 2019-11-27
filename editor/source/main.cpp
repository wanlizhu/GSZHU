#include <iostream>
#include "Wanlix/Core3D/Context.h"
#include "Wanlix/Core3D/Platform/Window.h"

using namespace Wanlix;

int main()
{
    WindowDescriptor desc;
    desc.title = L"Wanlix Editor";
    auto win = Window::Create(desc);
    win->Show();

    while (win->Tick()) {}
    

    return 0;
}