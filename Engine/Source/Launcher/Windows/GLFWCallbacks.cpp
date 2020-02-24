#ifdef PLATFORM_WINDOWS
#include "Launcher/Windows/GLFWCallbacks.h"
#include "Utils/Log.h"
#include "Events/Event.h"
#include "GLFW/glfw3.h"

namespace Wanli
{
    void OnError_GLFW(int32_t error, const char* description)
    {
        LOG_ERROR("GLFW error: %s\n", description);
    }

    void OnWindowPos_GLFW(GLFWwindow* window, int32_t xpos, int32_t ypos)
    {
        Event::Create(EVT_WindowPos, xpos, ypos)->PostEvent();
    }

    void OnWindowSize_GLFW(GLFWwindow* window, int32_t width, int32_t height)
    {
        Event::Create(EVT_WindowSize, width, height)->PostEvent();
    }

    void OnWindowClose_GLFW(GLFWwindow* window)
    {
        Event::Create(EVT_WindowClose)->PostEvent();
    }

    void OnWindowFocus_GLFW(GLFWwindow* window, int32_t focused)
    {
        Event::Create(EVT_WindowFocus, focused)->PostEvent();
    }

    void OnWindowIconified_GLFW(GLFWwindow* window, int32_t iconified)
    {
        Event::Create(EVT_WindowIconified)->PostEvent();
    }

    void OnFramebufferSize_GLFW(GLFWwindow* window, int32_t width, int32_t height)
    {
        Event::Create(EVT_FramebufferSize, width, height)->PostEvent();
    }
}
#endif