#include <iostream>
#include "Wanlix/Core3D/Context.h"
#include "Wanlix/PAL/Window.h"

using namespace Wanlix;

int main()
{
    WindowDescriptor desc;
    desc.title = L"Wanlix Editor";
    auto win = Window::Create(desc);
    win->Show();

    while (win->ProcessEvents())
    {
    
    }
 
    return 0;
}