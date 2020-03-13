#ifndef __ANDROID__
#include "Core/Common.h"
#include "Window/Windows/GLFW_WindowCallbacks.h"
#include "Utils/Log.h"
#include "Events/Windows/WindowEvent.h"
#include "Events/Windows/KeyboardEvent.h"
#include "Events/Windows/MouseEvent.h"
#include "GLFW/glfw3.h"

namespace Wanli
{
    void OnError_GLFW(int32_t error, const char* description)
    {
        LOG_ERROR("GLFW error: %s\n", description);
    }

    void OnWindowPos_GLFW(GLFWwindow* window, int32_t xpos, int32_t ypos)
    {
        EventPool::Allocate<WindowPosEvent>(glm::ivec2(xpos, ypos))->PostEvent();
    }

    void OnWindowSize_GLFW(GLFWwindow* window, int32_t width, int32_t height)
    {
        EventPool::Allocate<WindowResizeEvent>(glm::ivec2(width, height))->PostEvent();
    }

    void OnWindowClose_GLFW(GLFWwindow* window)
    {
        EventPool::Allocate<WindowCloseEvent>()->PostEvent();
    }

    void OnWindowIconified_GLFW(GLFWwindow* window, int32_t iconified)
    {
        if (iconified)
        {
            EventPool::Allocate<WindowIconifiedEvent>()->PostEvent();
        }
    }
}

#endif