#include "window/GLWindow.h"
#include "scene/Scene.h"

using namespace NTGS;

int main() 
{
    auto win = GLWindow::Create("OpenGL", Vec4i(-1, -1, 1000, 618));
    auto scene = Scene::Create("");

    scene->SetDisplayWindow(win);
    win->MainLoop();

    return 0;
}