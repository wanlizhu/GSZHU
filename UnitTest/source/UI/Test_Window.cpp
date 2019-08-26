#include "gtest/gtest.h"
#include <iostream>
#include <memory>
#include "Utils/Window.h"

using namespace GS;

TEST(Test_Window, Create)
{
	Window::Desc desc;
	desc.Title = "Test Window";
	auto win = Window::Create(desc, nullptr);
	win->MessageLoop();
}