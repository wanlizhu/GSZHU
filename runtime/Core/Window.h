#pragma once

#include "Config.h"
#include "Common.h"
#include "Utils/NonCopyable.h"
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

    class DLL_DECL Window : public NonCopyable
    {
        friend class GLFWCallbacks;
    public:
        #ifdef _WIN32
        using Handle = HWND;
        #else
        using Handle = void*;
        #endif
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
        inline Handle GetHandle() const { return mHandle; }
        inline Uint2 GetClientAreaSize() const { return { mDesc.width, mDesc.height }; }
        inline const WindowDesc& GetDesc() const { return mDesc; }

    private:
        Window(const WindowDesc& desc, ICallbacks* callbacks);
        void CheckWindowSize();

    private:
        WindowDesc mDesc;
        GLFWwindow* mGLFWwindow = nullptr;
        Handle mHandle = nullptr;
        Uint2 mMouseScale;
        ICallbacks* mCallbacks = nullptr;
    };
}