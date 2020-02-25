#pragma once
#ifdef PLATFORM_WINDOWS

#include "Window/Windows/Monitor.h"
#include "Window/Windows/Menu.h"
#include "Window/Windows/Cursor.h"
#include "Events/Windows/WindowEvent.h"
#include "Events/Windows/KeyboardEvent.h"
#include "Events/Windows/MouseEvent.h"
#include "Utils/Delegate.h"
#include "Core/IModule.h"

struct GLFWwindow;

namespace Wanli
{
    class Window : public IModule::Registrar<Window, EModuleStage::PreUpdate>
    {
    public:
        using PTR = std::shared_ptr<Window>;
        using MessageHook = std::function<LRESULT(HWND, UINT, WPARAM, LPARAM)>;

        Window() = default;
        virtual ~Window();
        
        virtual void Initialize() override;
        virtual void Update() override;
        virtual void Destroy() override;

        void Show();
        void Hide();
        void Close();
        bool ShouldClose();
        
        GLFWwindow* GetGLFW();
        Monitor& GetMonitor();
        Cursor& GetCursor();
        Menu& GetMenu();
        HWND GetHwnd() const;
        glm::ivec2 GetSize() const;
        glm::ivec2 GetPos() const;
        int GetAttribute(int attrib) const;

        void SetSize(const glm::ivec2& size);
        void SetPos(const glm::ivec2& pos);
        void SetTitle(const char* name);
        void SetIcons(const Array<Path>& paths);
        void SetFullscreen(bool value);
        void SetAttribute(int attrib,int value);
        void AddMessageHook(const MessageHook& hook);

    protected:
        static LRESULT CALLBACK WindowMessageProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);
        void CreateMenu();

    protected:
        GLFWwindow* mWindowGLFW = nullptr;
        Monitor mMonitor;
        Cursor mCursor;
        Menu mMenu;
        HWND mHwnd = 0;
        Array<MessageHook> mMessageHooks;

        bool mFullscreen = false;
        glm::ivec2 mRestorePos;
        glm::ivec2 mRestoreSize;
    };
}

#endif