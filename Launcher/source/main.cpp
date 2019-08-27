#include <iostream>
#include "Utils/Window.h"

using namespace GS;

int main()
{
	Window::Desc desc;
	desc.Title = "Test Window";
	desc.Backend = Window::EBackend::OpenGL;
	auto win = Window::Create(desc, nullptr);
	win->MessageLoop();

    return 0;
}
