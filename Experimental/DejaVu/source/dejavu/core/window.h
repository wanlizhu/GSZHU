#pragma once

#include <memory>
#include <string>
#include <vector>
#include <array>
#include <chrono>
#include <thread>
#include "utils/preprocess.h"
#include "utils/timer.h"
#include "device/device_config.h"
#ifdef _WIN32
#define NOMINMAX
#define WIN32_LEAN_AND_MEAN 
#include <Windows.h>
using WindowHandle = HWND;
#else
using WindowHandle = void*;
#endif

struct GLFWwindow;

namespace djv
{
    struct KeyboardEvent;
    struct MouseEvent;

    enum class WindowFlag
    {
        None = 0,
        Resizable = 1,
        Borderless = 1 << 1,
        Floating = 1 << 2,
    };
    ENUM_CLASS_OP(WindowFlag)

    class WindowCallbacks 
    {
    public:
        virtual void windowSizeChanged(int w, int h) = 0;
        virtual void windowRedraw() = 0;
        virtual void windowClose() = 0 ;
        virtual void keyboardEvent(const KeyboardEvent& keyboardEvent) = 0;
        virtual void mouseEvent(const MouseEvent& mouseEvent) = 0;
    };

    class DJV_API Window
    {
    public:
        Window(
            const wchar_t* title, 
            int width, 
            int height, 
            std::weak_ptr<WindowCallbacks> callbacks, 
            WindowFlag flags = WindowFlag::None
        );

        void messageLoop(); /* Start executing the messageLoop. The only way to stop it is to call close() */
        void close(); /* This will cause the messageLoop() to stop its execution */
        void resize(int width, int height); /* There is not guarantee that the call will succeed. */
        void pollEvents(); /* Force event polling. Useful if your rendering loop is slow and you would like to get a recent keyboard/mouse status. */
        void setWindowPos(int x, int y);
        void setWindowTitle(const wchar_t* title);
        void setCallbacks(std::weak_ptr<WindowCallbacks> callbacks);
        void setTickTimer(std::chrono::milliseconds ms);

        const wchar_t* getWindowTitle() const;
        WindowHandle getWindowHandle() const;
        std::array<int, 2> getClientAreaSize() const;
        WindowCallbacks* getCallbacks() const;
        
    private:
        std::wstring mWindowTitle;
        std::shared_ptr<GLFWwindow> mGLFWWindow;
        std::array<float, 2> mMouseScale;
        std::weak_ptr<WindowCallbacks> mCallbacks;
        Timer mTickTimer;
    };
}