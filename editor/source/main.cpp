#include <iostream>
#include "Wanlix/Platform/IWindow.h"

using namespace Wanlix;

int main()
{
    WindowDescriptor desc;
    desc.title = L"Wanlix Editor";
    auto win = IWindow::Create(desc);
    win->Show();

    

    return 0;
}