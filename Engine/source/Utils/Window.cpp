#include "Utils/Window.h"
#include <cassert>
#ifdef _WIN32
#define GLFW_EXPOSE_NATIVE_WIN32
#else
#define GLFW_EXPOSE_NATIVE_X11
#endif
#include "GLFW/glfw3.h"
#include "GLFW/glfw3native.h"

namespace GS
{
	namespace local
	{
		class EventTransfer
		{
		public:
			static void OnWindowSize(GLFWwindow* win, int w, int h)
			{
				if (w == 0 || h == 0)
					return;

				Window* window = (Window*)glfwGetWindowUserPointer(win);
				if (window != nullptr && window->mpCallbacks != nullptr)
					window->mpCallbacks->OnResize(w, h);
			}


		};
	} // namespace local

	void Window::Initialize()
	{
		static bool _inited = false;
		if (!_inited)
		{
			_inited = true;
			
		}
	}

	Window::Window(const std::string& name)
		: Object(name)
	{
		Window::Initialize();
	}

	Window::~Window()
	{
		Window::Destroy();
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
		assert(glfwInit() == GLFW_TRUE);

		glfwSetErrorCallback([](int code, const char* str) {
#if LOG_ENABLED
			LOG(ERROR) << "Error(" << code << "):" << str;
#endif
		});

		glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
		GLFWmonitor* monitor = desc.IsFullScreen ? glfwGetPrimaryMonitor() : nullptr;
		const GLFWvidmode* mode = glfwGetVideoMode(monitor);

		int width = desc.Size[0] <= 0 ? Window::DefaultSize()[0] : desc.Size[0];
		int height = desc.Size[1] <= 0 ? Window::DefaultSize()[1] : desc.Size[1];
		GLFWwindow* window = glfwCreateWindow(width, height, desc.Title.c_str(), monitor, nullptr);
		assert(window != nullptr);

		glfwSetWindowPos(window, desc.Position[0], desc.Position[1]);

		static int64_t _id = 0;
		Window::SharedPtr shared(new Window("Window" + std::to_string(_id++)));
		shared->mpWindow = window;
		shared->mpCallbacks = callbacks;
		shared->ComputeMouseScale();
		glfwSetWindowUserPointer(shared->mpWindow, shared.get());


		glfwSetWindowSizeCallback(window, local::EventTransfer::OnWindowSize);
		glfwSetKeyCallback(window, local::EventTransfer::);
		glfwSetMouseButtonCallback(window, local::EventTransfer::);
		glfwSetCursorPosCallback(window, local::EventTransfer::);
		glfwSetScrollCallback(window, local::EventTransfer::);
		glfwSetCharCallback(window, local::EventTransfer::);
		glfwSetDropCallback(window, local::EventTransfer::);

		return shared;
	}

	void Window::ComputeMouseScale()
	{
		assert(mpWindow != nullptr);

		int32_t actualWidth;
		int32_t actualHeight;
		glfwGetWindowSize(mpWindow, &actualWidth, &actualHeight);

		mMouseScale[0] = 1.0f / (float)actualWidth;
		mMouseScale[1] = 1.0f / (float)actualHeight;
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
#ifdef _WIN32
		return glfwGetWin32Window(mpWindow);
#else
		glfwGetX11Window(pGLFWWindow);
#endif
	}

	std::array<float, 2> Window::GetMouseScale() const
	{
		return mMouseScale;
	}
}