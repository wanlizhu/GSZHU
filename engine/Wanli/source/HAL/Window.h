#pragma once

#include "Core/BasicTypes.h"
#include "Core/IModule.h"

struct GLFWwindow;

namespace Wanli
{
    struct WindowCreateInfo
    {
        String title = "Wanli Engine";
        glm::ivec2 size = { 1920, 1080 };
        bool fullscreen = false;
        bool resizable = true;
        bool borderless = false;
        bool floating = false;
    };

    class DLLDECL Window : public IModule
    {
    public:

    public:
        Delegate<void(glm::ivec2)> OnResize;
        Delegate<void(bool)> OnFullscreen;
        Delegate<void()> OnClose;
        Delegate<void(bool)> OnIconified;

    protected:
        WindowCreateInfo mCreateInfo;
        GLFWwindow* mWindowGLFW = nullptr;
        glm::vec2 mMouseScale;
        bool mClosed = false;
        bool mFocused = false;
        bool mIconified = false;
    };
}