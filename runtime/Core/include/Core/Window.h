#pragma once

#include "Common.h"
#ifdef _WIN32
#include <comdef.h>
#endif

struct GLFWwindow;

namespace Wanlix
{
    struct KeyEvent;
    struct MouseEvent;

    struct WindowDesc
    {
        Uint width = 1920;
        Uint height = 1080;
        Bool fullscreen = false;
        String title = "Wanlix Engine";
        Bool resizable = true;
    };

    class Window
    {
        friend class GLFWCallbacks;
    public:
        class ICallbacks
        {
        public:
            virtual void OnSizeChanged() = 0;
            virtual void OnDraw() = 0;
            virtual void OnKeyEvent(const KeyEvent& event) = 0;
            virtual void OnMouseEvent(const MouseEvent& event) = 0;
            virtual void OnDroppedFile(const String& filename) = 0;
        };

        static SharedPtr<Window> Create(const WindowDesc& desc, ICallbacks* callbacks);
        
        ~Window();
        void Close();
        void SetSize(Uint w, Uint h);
        void MessageLoop();
        void PollEvents();
        void SetTitle(const String& title);
        inline WindowHandle GetHandle() const { return mHandle; }
        inline Uint2 GetClientAreaSize() const { return { mDesc.width, mDesc.height }; }
        inline const WindowDesc& GetDesc() const { return mDesc; }

    private:
        Window(const WindowDesc& desc, ICallbacks* callbacks);
        Window(const Window&) = delete;
        Window& operator=(const Window&) = delete;
        void CheckWindowSize();

    private:
        WindowDesc mDesc;
        GLFWwindow* mGLFWwindow = nullptr;
        WindowHandle mHandle = nullptr;
        Uint2 mMouseScale;
        ICallbacks* mCallbacks = nullptr;
    };
}