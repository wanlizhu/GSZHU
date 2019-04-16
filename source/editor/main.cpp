#include "window/GLWindow.h"

using namespace NTGS;

int main() 
{
    auto win = GLWindow::Create("OpenGL", Vec4i(-1, -1, 1000, 618));
    win->MainLoop();

    return 0;
}