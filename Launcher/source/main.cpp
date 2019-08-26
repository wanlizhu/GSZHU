#include <iostream>
#include "Utils/Window.h"

using namespace GS;

int main()
{
	Window::Desc desc;
	desc.Title = "Test Window";
	auto win = Window::Create(desc, nullptr);
	win->MessageLoop();

    return 0;
}
