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
		static void OnWindowSize(GLFWwindow* win, int w, int h)
		{
			if (w == 0 || h == 0)
				return;

			Window* window = (Window*)glfwGetWindowUserPointer(win);
			if (window != nullptr && window->GetCallbacks() != nullptr)
				window->GetCallbacks()->OnResize(w, h);
		}

		static void OnKey(GLFWwindow* win, int key, int scanCode, int action, int mods)
		{
			static auto _GetModsFunc = [](int mask) {
				InputModifiers mods;
				mods.Alt = (mask & GLFW_MOD_ALT) != 0;
				mods.Ctrl = (mask & GLFW_MOD_CONTROL) != 0;
				mods.Shift = (mask & GLFW_MOD_SHIFT) != 0;
				return mods;
			};

			Window* window = (Window*)glfwGetWindowUserPointer(win);
			if (window != nullptr && window->GetCallbacks() != nullptr)
			{
				if (action == GLFW_REPEAT || key == GLFW_KEY_UNKNOWN)
					return;

				KeyboardEvent event;
				event.Key = key;
				event.Mods = _GetModsFunc(mods);
				event.Type = (action == GLFW_RELEASE ? KeyboardEvent::EType::Up : KeyboardEvent::EType::Down);
				window->GetCallbacks()->OnKeyboardEvent(event);
			}
		}

		static void OnChar(GLFWwindow* win, uint32_t input)
		{
			Window* window = (Window*)glfwGetWindowUserPointer(win);
			if (window != nullptr && window->GetCallbacks() != nullptr)
			{
				KeyboardEvent event;
				event.Type = KeyboardEvent::EType::Char;
				event.CodePoint = input;

				window->GetCallbacks()->OnKeyboardEvent(event);
			}
		}

		static void OnMouseMove(GLFWwindow* win, double x, double y)
		{
			Window* window = (Window*)glfwGetWindowUserPointer(win);
			if (window != nullptr && window->GetCallbacks() != nullptr)
			{
				MouseEvent event;
				event.Type = MouseEvent::EType::Move;
				event.Position = { 
					(int)(x * window->GetMouseScale()[0]),
					(int)(y * window->GetMouseScale()[1]) };

				window->GetCallbacks()->OnMouseEvent(event);
			}
		}

		static void OnMouseButton(GLFWwindow* win, int button, int action, int mods)
		{
			static auto _GetModsFunc = [](int mask) {
				InputModifiers mods;
				mods.Alt = (mask & GLFW_MOD_ALT) != 0;
				mods.Ctrl = (mask & GLFW_MOD_CONTROL) != 0;
				mods.Shift = (mask & GLFW_MOD_SHIFT) != 0;
				return mods;
			};

			Window* window = (Window*)glfwGetWindowUserPointer(win);
			if (window != nullptr && window->GetCallbacks() != nullptr)
			{
				MouseEvent event;
				switch (button)
				{
				case GLFW_MOUSE_BUTTON_LEFT:
					event.Type = (action == GLFW_PRESS ? MouseEvent::EType::LeftDown : MouseEvent::EType::LeftUp);
					break;

				case GLFW_MOUSE_BUTTON_MIDDLE:
					event.Type = (action == GLFW_PRESS ? MouseEvent::EType::MiddleDown : MouseEvent::EType::MiddleUp);
					break;

				case GLFW_MOUSE_BUTTON_RIGHT:
					event.Type = (action == GLFW_PRESS ? MouseEvent::EType::RightDown : MouseEvent::EType::RightUp);
					break;
				default:
					break;
				}

				event.Mods = _GetModsFunc(mods);
				double x, y;
				glfwGetCursorPos(win, &x, &y);
				event.Position = {
					(int)(x * window->GetMouseScale()[0]),
					(int)(y * window->GetMouseScale()[1]) };

				window->GetCallbacks()->OnMouseEvent(event);
			}
		}

		static void OnMouseWheel(GLFWwindow* win, double x, double y)
		{
			Window* window = (Window*)glfwGetWindowUserPointer(win);
			if (window != nullptr && window->GetCallbacks() != nullptr)
			{
				MouseEvent event;
				event.Type = MouseEvent::EType::Wheel;
				double x, y;
				glfwGetCursorPos(win, &x, &y);
				event.Position = {
					(int)(x * window->GetMouseScale()[0]),
					(int)(y * window->GetMouseScale()[1]) };
				event.WheelDelta = { (int)x, (int)y };

				window->GetCallbacks()->OnMouseEvent(event);
			}
		}

		static void OnDropFile(GLFWwindow* win, int count, const char** paths)
		{
			Window* window = (Window*)glfwGetWindowUserPointer(win);
			if (window != nullptr && window->GetCallbacks() != nullptr)
			{
				std::vector<std::string> vec;
				for (int i = 0; i < count; i++)
					vec.push_back(paths[i]);

				window->GetCallbacks()->OnDropFile(vec);
			}
		}

	} // namespace local

	class EmptyCallbacks : public Window::ICallbacks
	{
	public:
		virtual void OnCreate() override {}
		virtual void OnDestroy()  override {}
		virtual void OnResize(int width, int height)  override {}
		virtual void OnUpdate()  override {}
		virtual void OnRender()  override {}

		virtual void OnKeyboardEvent(const KeyboardEvent& event)  override {}
		virtual void OnMouseEvent(const MouseEvent& event)  override {}
		virtual void OnDropFile(const std::vector<std::string>& paths)  override {}
	};

	Window::Window(const std::string& name, Window::ICallbacks* callbacks)
		: Object(name)
		, mpCallbacks(callbacks)
	{
		assert(glfwInit() == GLFW_TRUE);
		if (mpCallbacks == nullptr)
		{
			static EmptyCallbacks _callbacks;
			mpCallbacks = &_callbacks;
		}
	}

	Window::~Window()
	{
		Window::Destroy();
	}

	std::array<int, 2> Window::DefaultPos()
	{
		assert(glfwInit() == GLFW_TRUE);

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
		assert(glfwInit() == GLFW_TRUE);

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
			LOG(ERROR) << "Error(" << code << "): " << str;
#endif
		});

		//glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
		GLFWmonitor* monitor = desc.IsFullScreen ? glfwGetPrimaryMonitor() : nullptr;
		const GLFWvidmode* mode = monitor ? glfwGetVideoMode(monitor) : nullptr;

		int width = desc.Size[0] <= 0 ? Window::DefaultSize()[0] : desc.Size[0];
		int height = desc.Size[1] <= 0 ? Window::DefaultSize()[1] : desc.Size[1];
		GLFWwindow* window = glfwCreateWindow(width, height, desc.Title.c_str(), monitor, nullptr);
		assert(window != nullptr);

		glfwSetWindowPos(window, desc.Position[0], desc.Position[1]);

		static int64_t _id = 0;
		Window::SharedPtr shared(new Window("Window" + std::to_string(_id++), callbacks));
		shared->mpWindow = window;
		shared->ComputeMouseScale();
		glfwSetWindowUserPointer(shared->mpWindow, shared.get());


		glfwSetWindowSizeCallback(window, local::OnWindowSize);
		glfwSetKeyCallback(window, local::OnKey);
		glfwSetMouseButtonCallback(window, local::OnMouseButton);
		glfwSetCursorPosCallback(window, local::OnMouseMove);
		glfwSetScrollCallback(window, local::OnMouseWheel);
		glfwSetCharCallback(window, local::OnChar);
		glfwSetDropCallback(window, local::OnDropFile);

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
		// Samples often rely on a size change event as part of initialization
		// This would have happened from a WM_SIZE message when calling ShowWindow on Win32
		int w, h;
		glfwGetWindowSize(mpWindow, &w, &h);
		mpCallbacks->OnResize(w, h);

		glfwShowWindow(mpWindow);
		glfwFocusWindow(mpWindow);

		while (!glfwWindowShouldClose(mpWindow))
		{
			glfwPollEvents();
			mpCallbacks->OnUpdate();
			mpCallbacks->OnRender();
			glfwSwapBuffers(mpWindow);
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

	Window::ICallbacks* Window::GetCallbacks() const
	{
		return mpCallbacks;
	}
}