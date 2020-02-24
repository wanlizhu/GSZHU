#pragma once
#ifdef PLATFORM_WINDOWS

#include "Application/IApplication.h"
#include "Application/Windows/Monitor.h"
#include "Application/Windows/Menu.h"
#include "Application/Windows/Cursor.h"
#include "Events/WindowEvent.h"
#include "Events/KeyboardEvent.h"
#include "Events/MouseEvent.h"
#include "Core/BasicTypes.h"
#include "Utilities/Delegate.h"
#ifdef ENABLE_VULKAN
#include <vulkan/vulkan.h>
#endif

struct GLFWwindow;

namespace Wanli
{

    struct WindowAttrbutes
    {
        bool fullscreen = false;
        bool resizable = true;
        bool borderless = false;
        bool floating = false;
    };

    struct WindowCreateInfo
    {
        String title = "Wanli Engine";
        glm::ivec2 pos = { -1, -1 };
        glm::ivec2 size = { 0 , 0 };

        bool createMenu = true;
        WindowAttrbutes attribs = {};
    };

    class Window : public NonCopyable
        , public std::enable_shared_from_this<Window>
    {
    public:
        using PTR = std::shared_ptr<Window>;

        explicit Window(WindowCreateInfo& info);
        virtual ~Window();
		
		virtual void OnWindowCreated() {}
        virtual void OnWindowClosing() {}
        virtual void OnWindowIconified() {}
        virtual void OnWindowEvent(WindowEvent::PTR) {}
        virtual void OnKeyboardEvent(KeyboardEvent::PTR) {}
        virtual void OnMouseEvent(MouseEvent::PTR) {}
        virtual void OnWindowMessage(MSG msg, WPARAM wParam, LPARAM lParam) {}
		
		int Run();
        void Show();
        void Hide();
        void Quit();

        GLFWwindow* GetGLFW();
        Monitor& GetMonitor();
        Cursor& GetCursor();
        Menu& GetMenu();
        glm::ivec2 GetSize() const;
        glm::ivec2 GetPos() const;
        const WindowAttrbutes& GetAttribs() const;

        void SetSize(const glm::ivec2& size);
        void SetPos(const glm::ivec2& pos);
        void SetTitle(const char* name);
        void SetIcons(const Array<Path>& paths);
        void SetIconified(bool iconified);
        void SetFloating(bool floating);
        void SetResizable(bool resizable);
        void SetBorderless(bool borderless);
        void SetFullscreen(bool fullscreen);
		
    protected:
        GLFWwindow* mWindowGLFW = nullptr;
        Monitor mMonitor;
        Cursor mCursor;
        Menu mMenu;

        WindowAttrbutes mAttribs;
    };
}

#endif