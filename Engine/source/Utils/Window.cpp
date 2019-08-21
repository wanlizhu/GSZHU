#include "Utils/Window.h"
#include <cassert>
#define GLFW_EXPOSE_NATIVE_WIN32
#include "GLFW/glfw3.h"
#include "GLFW/glfw3native.h"

namespace GS
{
	void Window::Initialize()
	{
		static bool _inited = false;
		if (!_inited)
		{
			_inited = true;
			glfwInit();
			glfwSetErrorCallback([](int code, const char* str) {
#if LOG_ENABLED
				LOG(ERROR) << "Error(" << code << "):" << str;
#endif
			});
		}
	}

	Window::Window()
	{
		Window::Initialize();
	}

	Window::~Window()
	{
		Destroy();
	}

	std::array<int, 2> Window::DefaultPos()
	{
		Window::Initialize();

		GLFWmonitor* monitor = glfwGetPrimaryMonitor();
		const GLFWvidmode* mode = glfwGetVideoMode(monitor);
		const std::array<int, 2>& size = Window::DefaultSize();

		return std::array<int, 2> {
			(mode->width - size[0]) / 2 , 
			(mode->height - size[1]) / 2 
		};
	}

	std::array<int, 2> Window::DefaultSize()
	{
		Window::Initialize();

		GLFWmonitor* monitor = glfwGetPrimaryMonitor();
		const GLFWvidmode* mode = glfwGetVideoMode(monitor);

		return std::array<int, 2> {
			int(mode->width / 1.414),
			int(mode->height / 1.414)
		};
	}

	Window::SharedPtr Window::Create(const Window::Desc& desc, ICallbacks* callbacks)
	{
		GLFWmonitor* monitor = glfwGetPrimaryMonitor();
		const GLFWvidmode* mode = glfwGetVideoMode(monitor);
		GLFWwindow* window = nullptr;

		if (desc.IsFullScreen)
		{
			glfwWindowHint(GLFW_RED_BITS, mode->redBits);
			glfwWindowHint(GLFW_GREEN_BITS, mode->greenBits);
			glfwWindowHint(GLFW_BLUE_BITS, mode->blueBits);
			glfwWindowHint(GLFW_REFRESH_RATE, mode->refreshRate);

			window = glfwCreateWindow(mode->width, mode->height, desc.Title.c_str(), monitor, nullptr);
			assert(window != nullptr);
		}
		else
		{
			int width = desc.Size[0] <= 0 ? Window::DefaultSize()[0] : desc.Size[0];
			int height = desc.Size[1] <= 0 ? Window::DefaultSize()[1] : desc.Size[1];
			window = glfwCreateWindow(width, height, desc.Title.c_str(), nullptr, nullptr);
			assert(window != nullptr);
			glfwSetWindowPos(window, desc.Position[0], desc.Position[1]);
		}

		SharedPtr shared = std::make_shared<Window>();
		shared->mpWindow = window;

		return shared;
	}

	void Window::SetCallbacks(ICallbacks* callbacks)
	{
		mpCallbacks = callbacks;
	}

	void Window::SetTitle(const std::string& title)
	{
		glfwSetWindowTitle(mpWindow, title.c_str());
	}

	void Window::SetFullScreen(bool enabled)
	{
		static int _normalWindow[4] = { 0, 0, 0, 0 };

		if (enabled)
		{
			glfwGetWindowPos(mpWindow, &_normalWindow[0], &_normalWindow[1]);
			glfwGetWindowSize(mpWindow, &_normalWindow[2], &_normalWindow[3]);

			GLFWmonitor* monitor = glfwGetPrimaryMonitor();
			const GLFWvidmode* mode = glfwGetVideoMode(monitor);
			glfwSetWindowMonitor(mpWindow, monitor, 0, 0, mode->width, mode->height, mode->refreshRate);
		}
		else 
		{
			if (glfwGetWindowMonitor(mpWindow) != nullptr)
			{
				glfwSetWindowMonitor(mpWindow, NULL, _normalWindow[0], _normalWindow[1], _normalWindow[2], _normalWindow[3], 0);
			}
		}
	}

	void Window::SetSize(int width, int height)
	{
		glfwSetWindowSize(mpWindow, width, height);
	}

	void Window::MoveTo(int x, int y)
	{
		glfwSetWindowPos(mpWindow, x, y);
	}

	void Window::MessageLoop()
	{
		while (!glfwWindowShouldClose(mpWindow))
		{
			glfwSwapBuffers(mpWindow);
			glfwPollEvents();
		}
	}

	void Window::SetMessageHook(MSG_ID msg, const MSG_FUNC& func)
	{

	}

	void Window::Hide()
	{
		glfwHideWindow(mpWindow);
	}

	void Window::Show()
	{
		glfwShowWindow(mpWindow);
	}

	void Window::Destroy()
	{
		if (mpWindow)
		{
			glfwDestroyWindow(mpWindow);
			mpWindow = nullptr;
		}
	}


	std::string Window::GetTitle() const
	{
		static char _title[MAX_PATH] = { 0 };
		HWND hwnd = glfwGetWin32Window(mpWindow);
		GetWindowTextA(hwnd, _title, MAX_PATH);
		return _title;
	}

	std::array<int, 2> Window::GetPosition() const
	{
		static int _pos[2] = { 0 };
		glfwGetWindowPos(mpWindow, &_pos[0], &_pos[1]);
		return { _pos[0], _pos[1] };
	}

	std::array<int, 2> Window::GetSize() const
	{
		static int _size[2] = { 0 };
		glfwGetWindowSize(mpWindow, &_size[0], &_size[1]);
		return { _size[0], _size[1] };
	}

	WindowHandle Window::GetWindowHandle() const
	{
		return glfwGetWin32Window(mpWindow);
	}
}