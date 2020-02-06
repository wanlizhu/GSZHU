#include <iostream>
#include "Wanli/Core3D/Context.h"
#include "Wanli/PAL/Window.h"

using namespace Wanli;

int main()
{
    WindowDescriptor desc;
    desc.title = L"Wanli Editor";
    auto win = Window::Create(desc);
    win->Show();

    while (win->ProcessEvents())
    {
    
    }
 
    return 0;
}