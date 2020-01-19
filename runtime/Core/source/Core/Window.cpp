#include "Core/Window.h"
#include "Utils/UI/UserInput.h"
#include "Utils/Logger.h"

// Don't include GL/GLES headers
#define GLFW_INCLUDE_NONE

#ifdef _WIN32
#define GLFW_EXPOSE_NATIVE_WIN32
#include "GLFW/glfw3.h"
#include "GLFW/glfw3native.h"
#else
#define None 0L
#define Bool int
#define Status int
#define Always 2
#define GLFW_EXPOSE_NATIVE_X11
#include "GLFW/glfw3.h"
#include "GLFW/glfw3native.h"
#endif

namespace Wanlix
{
    class GLFWCallbacks
    {
    public:
        static void WindowSizeCallback(GLFWwindow* win, int width, int height)
        {
            if (width == 0 || height == 0) {
                return;
            }

            Window* window = (Window*)glfwGetWindowUserPointer(win);
            if (window != nullptr) {
                window->SetSize(width, height);
            }
        }

        static void KeyboardCallback(GLFWwindow* win, int key, int scancode, int action, int modifiers)
        {
            KeyEvent event;
            event.type = KeyEvent::EType::Pressed;

            if (PrepareKeyEvent(key, action, modifiers, event))
            {
                Window* window = (Window*)glfwGetWindowUserPointer(win);
                if (window != nullptr) {
                    window->mCallbacks->OnKeyEvent(event);
                }
            }
        }

        static void CharInputCallback(GLFWwindow* win, Uint input)
        {
            KeyEvent event;
            event.type = KeyEvent::EType::Input;
            event.codepoint = input;

            Window* window = (Window*)glfwGetWindowUserPointer(win);
            if (window != nullptr) {
                window->mCallbacks->OnKeyEvent(event);
            }
        }

        static void MouseMoveCallback(GLFWwindow* win, double mouseX, double mouseY)
        {
            Window* window = (Window*)glfwGetWindowUserPointer(win);
            if (window != nullptr)
            {
                // Prepare the mouse data
                MouseEvent event;
                event.type = MouseEvent::EType::Move;
                event.pos = ComputeMousePos(mouseX, mouseY, window->mMouseScale);
                event.screenPos = { mouseX, mouseY };
                event.wheelDelta = Float2(0, 0);

                window->mCallbacks->OnMouseEvent(event);
            }
        }

        static void MouseButtonCallback(GLFWwindow* win, int button, int action, int modifiers)
        {
            MouseEvent event;
            // Prepare the mouse data
            switch (button)
            {
            case GLFW_MOUSE_BUTTON_LEFT:
                event.type = (action == GLFW_PRESS) ? MouseEvent::EType::LeftButtonDown : MouseEvent::EType::LeftButtonUp;
                break;
            case GLFW_MOUSE_BUTTON_MIDDLE:
                event.type = (action == GLFW_PRESS) ? MouseEvent::EType::MiddleButtonDown : MouseEvent::EType::MiddleButtonUp;
                break;
            case GLFW_MOUSE_BUTTON_RIGHT:
                event.type = (action == GLFW_PRESS) ? MouseEvent::EType::RightButtonDown : MouseEvent::EType::RightButtonUp;
                break;
            default:
                // Other keys are not supported
                break;
            }

            Window* window = (Window*)glfwGetWindowUserPointer(win);
            if (window != nullptr)
            {
                // Modifiers
                event.mods = GetInputModifiers(modifiers);
                double x, y;
                glfwGetCursorPos(win, &x, &y);
                event.pos = ComputeMousePos(x, y, window->mMouseScale);

                window->mCallbacks->OnMouseEvent(event);
            }
        }

        static void MouseWheelCallback(GLFWwindow* win, double scrollX, double scrollY)
        {
            Window* window = (Window*)glfwGetWindowUserPointer(win);
            if (window != nullptr)
            {
                MouseEvent event;
                event.type = MouseEvent::EType::Wheel;
                double x, y;
                glfwGetCursorPos(win, &x, &y);
                event.pos = ComputeMousePos(x, y, window->mMouseScale);
                event.wheelDelta = (glm::vec2(float(scrollX), float(scrollY)));

                window->mCallbacks->OnMouseEvent(event);
            }
        }

        static void ErrorCallback(int errorCode, const char* pDescription)
        {
            std::string errorMsg = std::to_string(errorCode) + " - " + std::string(pDescription) + "\n";
            LOG_ERROR(errorMsg.c_str());
        }

        static void DroppedFileCallback(GLFWwindow* win, int count, const char** paths)
        {
            Window* window = (Window*)glfwGetWindowUserPointer(win);
            if (window)
            {
                for (int i = 0; i < count; i++)
                {
                    std::string filename(paths[i]);
                    window->mCallbacks->OnDroppedFile(filename);
                }
            }
        }

    private:
        static inline InputModifiers GetInputModifiers(int mask)
        {
            InputModifiers mods;
            mods.alt = (mask & GLFW_MOD_ALT) != 0;
            mods.ctrl = (mask & GLFW_MOD_CONTROL) != 0;
            mods.shift = (mask & GLFW_MOD_SHIFT) != 0;
            return mods;
        }

        static inline Float2 ComputeMousePos(double xPos, double yPos, const Float2& mouseScale)
        {
            Float2 pos = Float2(float(xPos), float(yPos));
            pos *= mouseScale;
            return pos;
        }

        static inline bool PrepareKeyEvent(int key, int action, int modifiers, KeyEvent& event)
        {
            if (action == GLFW_REPEAT || key == GLFW_KEY_UNKNOWN)
            {
                return false;
            }

            event.type = (action == GLFW_RELEASE ? KeyEvent::EType::Released : KeyEvent::EType::Pressed);
            event.key = ToUniqueKey(key);
            event.mods = GetInputModifiers(modifiers);
            return true;
        }

        static inline EKey ToUniqueKey(int glfwKey)
        {
            static_assert(GLFW_KEY_ESCAPE == 256, "GLFW_KEY_ESCAPE is expected to be 256");
            if (glfwKey < GLFW_KEY_ESCAPE)
            {
                // Printable keys are expected to have the same value
                return (EKey)glfwKey;
            }

            switch (glfwKey)
            {
            case GLFW_KEY_ESCAPE:
                return EKey::Escape;
            case GLFW_KEY_ENTER:
                return EKey::Enter;
            case GLFW_KEY_TAB:
                return EKey::Tab;
            case GLFW_KEY_BACKSPACE:
                return EKey::Backspace;
            case GLFW_KEY_INSERT:
                return EKey::Insert;
            case GLFW_KEY_DELETE:
                return EKey::Del;
            case GLFW_KEY_RIGHT:
                return EKey::Right;
            case GLFW_KEY_LEFT:
                return EKey::Left;
            case GLFW_KEY_DOWN:
                return EKey::Down;
            case GLFW_KEY_UP:
                return EKey::Up;
            case GLFW_KEY_PAGE_UP:
                return EKey::PageUp;
            case GLFW_KEY_PAGE_DOWN:
                return EKey::PageDown;
            case GLFW_KEY_HOME:
                return EKey::Home;
            case GLFW_KEY_END:
                return EKey::End;
            case GLFW_KEY_CAPS_LOCK:
                return EKey::CapsLock;
            case GLFW_KEY_SCROLL_LOCK:
                return EKey::ScrollLock;
            case GLFW_KEY_NUM_LOCK:
                return EKey::NumLock;
            case GLFW_KEY_PRINT_SCREEN:
                return EKey::PrintScreen;
            case GLFW_KEY_PAUSE:
                return EKey::Pause;
            case GLFW_KEY_F1:
                return EKey::F1;
            case GLFW_KEY_F2:
                return EKey::F2;
            case GLFW_KEY_F3:
                return EKey::F3;
            case GLFW_KEY_F4:
                return EKey::F4;
            case GLFW_KEY_F5:
                return EKey::F5;
            case GLFW_KEY_F6:
                return EKey::F6;
            case GLFW_KEY_F7:
                return EKey::F7;
            case GLFW_KEY_F8:
                return EKey::F8;
            case GLFW_KEY_F9:
                return EKey::F9;
            case GLFW_KEY_F10:
                return EKey::F10;
            case GLFW_KEY_F11:
                return EKey::F11;
            case GLFW_KEY_F12:
                return EKey::F12;
            case GLFW_KEY_KP_0:
                return EKey::Keypad0;
            case GLFW_KEY_KP_1:
                return EKey::Keypad1;
            case GLFW_KEY_KP_2:
                return EKey::Keypad2;
            case GLFW_KEY_KP_3:
                return EKey::Keypad3;
            case GLFW_KEY_KP_4:
                return EKey::Keypad4;
            case GLFW_KEY_KP_5:
                return EKey::Keypad5;
            case GLFW_KEY_KP_6:
                return EKey::Keypad6;
            case GLFW_KEY_KP_7:
                return EKey::Keypad7;
            case GLFW_KEY_KP_8:
                return EKey::Keypad8;
            case GLFW_KEY_KP_9:
                return EKey::Keypad9;
            case GLFW_KEY_KP_DECIMAL:
                return EKey::KeypadDel;
            case GLFW_KEY_KP_DIVIDE:
                return EKey::KeypadDivide;
            case GLFW_KEY_KP_MULTIPLY:
                return EKey::KeypadMultiply;
            case GLFW_KEY_KP_SUBTRACT:
                return EKey::KeypadSubtract;
            case GLFW_KEY_KP_ADD:
                return EKey::KeypadAdd;
            case GLFW_KEY_KP_ENTER:
                return EKey::KeypadEnter;
            case GLFW_KEY_KP_EQUAL:
                return EKey::KeypadEqual;
            case GLFW_KEY_LEFT_SHIFT:
                return EKey::LeftShift;
            case GLFW_KEY_LEFT_CONTROL:
                return EKey::LeftControl;
            case GLFW_KEY_LEFT_ALT:
                return EKey::LeftAlt;
            case GLFW_KEY_LEFT_SUPER:
                return EKey::LeftSuper;
            case GLFW_KEY_RIGHT_SHIFT:
                return EKey::RightShift;
            case GLFW_KEY_RIGHT_CONTROL:
                return EKey::RightControl;
            case GLFW_KEY_RIGHT_ALT:
                return EKey::RightAlt;
            case GLFW_KEY_RIGHT_SUPER:
                return EKey::RightSuper;
            case GLFW_KEY_MENU:
                return EKey::Menu;
            default:
                assert(false);
                return (EKey)0;
            }
        }

    };


    Window::Window(const WindowDesc& desc, ICallbacks* callbacks)
        : mDesc(desc)
        , mMouseScale(1.0f / desc.width, 1.0f / desc.height)
        , mCallbacks(callbacks)
    {}

    void Window::CheckWindowSize()
    {
        // Actual window size may be clamped to slightly lower than monitor resolution
        int actualWidth, actualHeight;
        glfwGetWindowSize(mGLFWwindow, &actualWidth, &actualHeight);

        mDesc.width = uint32_t(actualWidth);
        mDesc.height = uint32_t(actualHeight);
        mMouseScale.x = (Uint)(1.0f / (float)mDesc.width);
        mMouseScale.y = (Uint)(1.0f / (float)mDesc.height);
    }

    Window::~Window()
    {
        glfwDestroyWindow(mGLFWwindow);
        glfwTerminate();
    }

    void Window::Close()
    {
        glfwSetWindowShouldClose(mGLFWwindow, 1);
    }

    SharedPtr<Window> Window::Create(const WindowDesc& desc, ICallbacks* callbacks)
    {
        // Set error callback
        glfwSetErrorCallback(GLFWCallbacks::ErrorCallback);

        // Init GLFW
        if (glfwInit() == GLFW_FALSE)
        {
            LOG_ERROR("GLFW initialization failed");
            return nullptr;
        }

        SharedPtr<Window> window = SharedPtr<Window>(new Window(desc, callbacks));

        // Create the window
        glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
        uint32_t w = desc.width;
        uint32_t h = desc.height;

        if (desc.fullscreen)
        {
            glfwWindowHint(GLFW_DECORATED, GLFW_FALSE);
            auto mon = glfwGetPrimaryMonitor();
            auto mod = glfwGetVideoMode(mon);
            w = mod->width;
            h = mod->height;
        }

        GLFWwindow* glfwWindow = glfwCreateWindow(w, h, desc.title.c_str(), nullptr, nullptr);

        if (glfwWindow == nullptr)
        {
            LOG_ERROR("Window creation failed!");
            return nullptr;
        }

        // Init handles
        window->mGLFWwindow = glfwWindow;
        #ifdef _WIN32
        window->mHandle = glfwGetWin32Window(glfwWindow);
        assert(window->mHandle);
        #else
        window->mHandle.pDisplay = glfwGetX11Display();
        window->mHandle.window = glfwGetX11Window(glfwWindow);
        assert(window->mHandle.pDisplay != nullptr);
        #endif
        window->CheckWindowSize();

        glfwSetWindowUserPointer(glfwWindow, window.get());

        // Set callbacks
        glfwSetWindowSizeCallback(glfwWindow, GLFWCallbacks::WindowSizeCallback);
        glfwSetKeyCallback(glfwWindow, GLFWCallbacks::KeyboardCallback);
        glfwSetMouseButtonCallback(glfwWindow, GLFWCallbacks::MouseButtonCallback);
        glfwSetCursorPosCallback(glfwWindow, GLFWCallbacks::MouseMoveCallback);
        glfwSetScrollCallback(glfwWindow, GLFWCallbacks::MouseWheelCallback);
        glfwSetCharCallback(glfwWindow, GLFWCallbacks::CharInputCallback);
        glfwSetDropCallback(glfwWindow, GLFWCallbacks::DroppedFileCallback);

        return window;
    }

    void Window::SetSize(Uint width, Uint height)
    {
        glfwSetWindowSize(mGLFWwindow, width, height);
        CheckWindowSize();

        mCallbacks->OnSizeChanged();
    }

    void Window::MessageLoop()
    {
        // Samples often rely on a size change event as part of initialization
        // This would have happened from a WM_SIZE message when calling ShowWindow on Win32
        mCallbacks->OnSizeChanged();

        glfwShowWindow(mGLFWwindow);
        glfwFocusWindow(mGLFWwindow);

        while (glfwWindowShouldClose(mGLFWwindow) == false)
        {
            glfwPollEvents();
            mCallbacks->OnDraw();
        }
    }

    void Window::SetTitle(const String& title)
    {
        glfwSetWindowTitle(mGLFWwindow, title.c_str());
    }

    void Window::PollEvents()
    {
        glfwPollEvents();
    }
}